#include "pch.h"
#include "assaultcube.h"

bool AC::IsTeamGame()
{

	int* gameMode = (int*)(0x50F49C);
	Player* localPlayer = *(Player**)0x50F4F4;

	switch (*gameMode)
	{
	case 0:
	case 4:
	case 5:
	case 7:
	case 11:
	case 13:
	case 14:
	case 16:
	case 17:
	case 20:
	case 21:
		return true;
	default:
		return false;
	}
}
bool AC::IsEnemy(Player* e)
{
	Player* localPlayer = *(Player**)0x50F4F4;

	if (localPlayer->team == e->team)
		return false;
	else return true;
}

bool AC::isValidEnt(Player* ent)
{
	if (ent)
	{
		if ((ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0) && ent->health > 0)
		{
			return true;
		}
	}
	return false;
}