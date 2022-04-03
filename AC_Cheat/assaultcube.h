#pragma once
#include "Player.h"

namespace AC 
{
	inline Player* localPlayer = *(Player**)0x50F4F4;
	inline PlayersList* playersList = *(PlayersList**)0x50F4F8;
	inline int* gameMode = (int*)(0x50F49C);
	inline float* matrix = (float*)(0x501AE8);
	inline int* nbPlayers = (int*)(0x50F500);

	bool IsTeamGame();
	bool IsEnemy(Player* e);
	bool isValidEnt(Player* ent);

}


