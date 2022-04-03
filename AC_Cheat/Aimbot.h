#pragma once
#include "Player.h"
#include "geom.h"


void MoveCrosshairTo(Vector3 otherPlayerPos);
int FindVisibleEnemyPlayers(std::vector<Player*>& players, std::vector<Player*>& visiblePlayers, int nbOfPlayers);
Player* FindClosestPlayer(std::vector<Player*>& players, int nbOfPlayers);



