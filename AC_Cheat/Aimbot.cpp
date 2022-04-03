#include "pch.h"
#include <vector>
#include "Aimbot.h"
#include "Vector.h"
#include "Player.h"

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


int FindVisibleEnemyPlayers(std::vector<Player*>& players, std::vector<Player*>& visiblePlayers, int nbOfPlayers)
{
    int nbOfVisiblePlayers = 1;
    for (int i = 1; i < nbOfPlayers; i++)
    {
        //std::cout << i << "eme player is : " << playersList[i] << std::endl;

        if (players[i]->isVisible(players[0]) && players[i]->state == 0 && players[i]->team != players[0]->team)
        {
            visiblePlayers.push_back(players[i]);
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