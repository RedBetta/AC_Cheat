// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "Vector.h"
#include "Player.h"
#include "Draw.h"
#include "constants.h"


bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16]) // 3D to 2D
{
    //Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
    Vector4 clipCoords{
        pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12],
        pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13],
        pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14],
        pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15]
    };

    if (clipCoords.w < 0.1f)
        return false;

    //perspective division, dividing by clip.W = Normalized Device Coordinates
    Vector3 NDC{
        clipCoords.x / clipCoords.w,
        clipCoords.y / clipCoords.w,
        clipCoords.z / clipCoords.w
    };

    //Transform to window coordinates
    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}




//void MoveMouseTo(Vector2 dest)
//{
//    INPUT Input = { 0 };
//    Input.type = INPUT_MOUSE;
//    Input.mi.dwFlags = MOUSEEVENTF_MOVE;
//    Input.mi.dx = dest.x - windowWidth / 2;
//    Input.mi.dy = dest.y - windowHeight / 2;
//    SendInput(1, &Input, sizeof(INPUT));
//
//}




void MoveCrosshairTo(Vector3 otherPlayerPos)
{
    DWORD localPlayer = *(DWORD*)0x50F4F4;
    Vector3 localPlayerPos{
        *(float*)(localPlayer + 0x4),
        *(float*)(localPlayer + 0x8),
        *(float*)(localPlayer + 0xC)
    };

    *(float*)(localPlayer + 0x40) = calcYaw(localPlayerPos, otherPlayerPos) + 90.0f;
    *(float*)(localPlayer + 0x44) = calcPitch(localPlayerPos, otherPlayerPos);
}




int FindVisiblePlayers(std::vector<Player*>& playersList, std::vector<Player*>& visiblePlayers, int nbOfPlayers)
{
    int nbOfVisiblePlayers = 1;
    for (int i = 1; i < nbOfPlayers; i++)
    {
        //std::cout << i << "eme player is : " << playersList[i] << std::endl;

        if (playersList[i]->isVisible(playersList[0]) && playersList[i]->state == 0)
        {
            visiblePlayers.push_back(playersList[i]);
            nbOfVisiblePlayers += 1;
        }
    }
    return nbOfVisiblePlayers;
}

Player* FindClosestPlayer(std::vector<Player*>& players, int nbOfPlayers)
{
    Player* closestPlayer = players[1];
    float minDistance = getMagnitude(players[0]->headPosition, players[1]->headPosition);
    for (int i = 2; i < nbOfPlayers; i++)
    {
        float magnitude = getMagnitude(players[0]->headPosition, players[i]->headPosition);
        if (magnitude < minDistance)
        {
            minDistance = magnitude;
            closestPlayer = players[i];
        }
    }
    return closestPlayer;
}

void LeftClikcMouse()
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    Input.mi.time = 1000;
    SendInput(1, &Input, sizeof(INPUT));

}

void SetAmmo(Player* player, int nbOfAmmo)
{
    player->currentWeapon->ammoPtr->ammo = nbOfAmmo;
}

void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Nop(BYTE* dst, unsigned int size)
{
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memset(dst, 0x90, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void EnableRecoilHack()
{
    //player->currentWeapon->gunInfo->recoilMagicNumber = 0
    Patch((BYTE*)(0x463600 + 0x181), (BYTE*)"\xb8\x36\x31\x00\x00\xb9\x32\x00\x00\x00\xba\x88\xaa\x0e\x00", 15);
}

void DisableRecoilHack()
{
    Patch((BYTE*)(0x463600 + 0x181), (BYTE*)"\x8B\x16\x8B\x52\x14\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 15);
}


int ESPThread()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    DWORD modelViewMatrixAddr = 0x501AE8;
    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    HWND windowHandle = FindWindow(0, L"AssaultCube");
    HDC hdc = GetDC(windowHandle);
    float modelViewMatrix[16]{};



    DWORD entityList = *(DWORD*)0x0050F4F8;
    std::cout << "Entity List Addr : " << entityList << std::endl;

    // Local Player
    Player* localPlayer = *(Player**)0x0050F4F4;
    std::cout << "Local Player Health : " << localPlayer->healthGUI << std::endl;
    Vector3 localPlayerPos{};

    // Players list
    std::vector<Player*> playersList{ localPlayer };
    std::vector<Player*> visiblePlayers{ localPlayer };

    std::cout << "Number of Players : " << *(DWORD*)(0x50F500) << std::endl;

    // Recoil Hack
    EnableRecoilHack();

    while (!GetAsyncKeyState(VK_END))
    {
        memcpy(&modelViewMatrix, (PBYTE*)(modelViewMatrixAddr), sizeof(modelViewMatrix));

        Vector2 bodyPosOnScreen{};
        Vector2 headPosOnScreen{};

        DWORD nbOfPlayers = *(DWORD*)(0x50F500);

        if (playersList.size() != nbOfPlayers)
        {
            playersList.resize(nbOfPlayers);
        }

        for (DWORD i = 1; i < nbOfPlayers; i++)
        {


            Player* player = *((Player**)(entityList + 0x4 * i) + 0x0);


            if (player != NULL)
            {
                if (player != playersList[i])
                {
                    //std::cout << "Player addr " << i << " : " << player << std::endl;
                    playersList[i] = player;
                    //std::cout << player->healthGUI << " at addr : " << &(player->healthGUI) << std::endl;
                    //std::cout << "Pos ( " << player->bodyPosition.x << ", " << player->bodyPosition.y << " )\n";
                }



                // Wallhack
                if (WorldToScreen(player->bodyPosition, bodyPosOnScreen, modelViewMatrix))
                {
                    if (WorldToScreen(player->headPosition, headPosOnScreen, modelViewMatrix))
                    {
                        DrawESP(bodyPosOnScreen, headPosOnScreen, hdc, brush);

                    }
                    //Turns on snaplines
                    if (GetKeyState(VK_F2) & 1)
                    {
                        DrawLine((int)bodyPosOnScreen.x, (int)bodyPosOnScreen.y, hdc);
                    }

                    if (GetAsyncKeyState(VK_F3) & 1)
                    {
                        MoveCrosshairTo(player->bodyPosition);
                    }
                }
            }


        }
        // Aimbot
        //std::cout << "Finding visible players ... \n";
        int nbOfVisiblePlayers = FindVisiblePlayers(playersList, visiblePlayers, nbOfPlayers);

        //std::cout << "Number of visible players : " << nbOfVisiblePlayers << std::endl;
        if (nbOfVisiblePlayers > 1)
        {
            Player* priorityTarget = FindClosestPlayer(visiblePlayers, nbOfVisiblePlayers);
            //std::cout << "priority target addr : " << priorityTarget << std::endl;

            while (priorityTarget->state == 0 && priorityTarget->isVisible(localPlayer))
            {
                // Move crosshair to player
                MoveCrosshairTo(priorityTarget->headPosition);
                SetAmmo(localPlayer, 9999);
                // Shoot
                localPlayer->bAttack = 1;
            }
            localPlayer->bAttack = 0;
        }

        visiblePlayers.resize(1);

    }
    DeleteObject(brush);
    fclose(f);
    FreeConsole();

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ESPThread, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

