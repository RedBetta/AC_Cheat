// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <vector>

#include "Vector.h"
#include "Player.h"
#include "Hook.h"
#include "geom.h"
#include "Aimbot.h"
#include "ESP.h"
#include "Mem.h"



// GLOBAL VARS

// Hook
typedef BOOL(__stdcall* twglSwapBuffers) (HDC hdc);
twglSwapBuffers owglSwapBuffers;
twglSwapBuffers wglSwapBuffersGateway;

// Window
//const int windowWidth = 800;
//const int windowHeight = 600;

// ESP
GL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

void SetAmmo(Player* player, int nbOfAmmo)
{
    player->weapon->ammo->ammoInside = nbOfAmmo;
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



void Draw()
{
    HDC currentHDC = wglGetCurrentDC();
    if (!glFont.bBuilt || currentHDC != glFont.hdc)
    {
        glFont.Build(FONT_HEIGHT);
    }

    GL::SetupOrtho();

    ESP::Draw(glFont);

    GL::RestoreGL();
}


BOOL __stdcall hkwglSwapBuffers(HDC hdc)
{
    // ESP
    Draw();
    return wglSwapBuffersGateway(hdc);
}


void GetPlayersList(std::vector<Player*>& players, int nbPlayers)
{
    DWORD playersList = *(DWORD*)0x0050F4F8;
    for (int i = 1; i < nbPlayers; i++)
    {
        Player* player = *((Player**)(playersList + 0x4 * i) + 0x0);

        if (player != NULL)
        {
            if (player != players[i])
            {
                players[i] = player;
            }
        }
    }
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    // Creates a Hook object
    // Hooks the wglSwapBuffers exported by opengl32.dll
    // the destination is the hkwglSwapBuffers function which is the hack function
    // the PtrToGatewayFnPtr is wglSwapBuffersGateway ???
    // the length is 5 because thats the lenght of a jump instruction in 32 bits
    Hook SwapBuffersHook = Hook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);

    // Retrieves the stolen bytes
    // Creates the trampoline hook with src = addr of wglSwapBuffers, dst = hkwglSwapBuffers and len = 5
    // Each time the game calls wglSwapBuffers, it will also jump to our hack function
    SwapBuffersHook.Enable();

    
    HWND windowHandle = FindWindow(0, L"AssaultCube");
    HDC hdc = GetDC(windowHandle);
    // Players

    std::vector<Player*> players{ AC::localPlayer };
    std::vector<Player*> visiblePlayers{ AC::localPlayer };

    // Recoil Hack
    EnableRecoilHack();
    bool bHackActive = true;

    while (!GetAsyncKeyState(VK_END))
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHackActive = !bHackActive;
            if (!bHackActive)
            {
                DisableRecoilHack();
                SwapBuffersHook.Disable();
                SetAmmo(AC::localPlayer, 20);
            }
            else
            {
                EnableRecoilHack();
                SwapBuffersHook.Enable();
            }
        }

        if (bHackActive)
        {
            int nbOfPlayers = *(int*)(0x50F500);

            if (players.size() != nbOfPlayers)
            {
                players.resize(nbOfPlayers);
            }
            GetPlayersList(players, nbOfPlayers);

            // Aimbot
            int nbOfVisiblePlayers = Aimbot::FindVisibleEnemyPlayers(players, visiblePlayers, *AC::nbPlayers);

            if (nbOfVisiblePlayers > 1)
            {
                Player* priorityTarget = Aimbot::FindClosestPlayer(visiblePlayers, nbOfVisiblePlayers);
                while (priorityTarget->state == 0 && priorityTarget->isVisible(AC::localPlayer))
                {
                    // Move crosshair to player
                    Aimbot::MoveCrosshairTo(priorityTarget->headPosition);
                    SetAmmo(AC::localPlayer, 9999);
                    // Shoot
                    AC::localPlayer->bAttack = 1;
                }
                AC::localPlayer->bAttack = 0;
            }
            visiblePlayers.resize(1);
        }
    }
    SwapBuffersHook.Disable();
    fclose(f);
    FreeConsole();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackThread, hModule, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}