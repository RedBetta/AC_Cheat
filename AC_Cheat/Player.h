#pragma once
#include <stdint.h>
#include <Windows.h>
#include "Vector.h"

enum gameModes
{
	TEAMDEATHMATCH = 0,
	COOPEDIT,
	DEATHMATCH,
	SURVIVOR,
	TEAMSURVIVOR,
	CTF,
	PISTOLFRENZY,
	BOTTEAMDEATHMATCH,
	BOTDEATHMATCH,
	LASTSWISSSTANDING,
	ONESHOTONEKILL,
	TEAMONESHOTONEKİLL,
	BOTONESHOTONEKILL,
	HUNTTHEFLAG,
	ТEAMKEEРTHEFLAG,
	KEEPTHEFLAG,
	TEAMPF,
	TEAMLSS,
	BOTPISTOLFRENZY,
	Botlss,
	BOTTEAMSURVIVOR,
	BOTTEAMONESHOTONEKILL,
	NUM
};

// Created with ReClass.NET 1.2 by KN4CK3R
class Weapon
{
public:
	class VTable* vTable; //0x0000
	int32_t ID; //0x0004
	class N00000594* owner; //0x0008
	class N000005A8* info; //0x000C
	class Ammo* ammo; //0x0010
	class Ammo2* ammo2; //0x0014
	char pad_0018[4]; //0x0018
	int32_t nbShotsFiredWithoutInterruption; //0x001C
	int32_t isReloading; //0x0020
	char pad_0024[32]; //0x0024
}; //Size: 0x0044
static_assert(sizeof(Weapon) == 0x44);

class Player
{
public:
	uint32_t vTable; //0x0000
	Vector3 headPosition; //0x0004
	char _qsdfqsdf[36];
	Vector3 bodyPosition;
	Vector3 rotation;
	char qdqd[37];
	BYTE bScoping;
	char qdqds[134];
	__int32 health;
	__int32 armor;
	char qsdgqsd[292];
	BYTE bAttack;
	char name[16];
	char qdgqg[247];
	BYTE team;
	char qsdgf[11];
	BYTE state;
	char sfgsdfh[59];
	Weapon* weapon;
	char qsdgqsdg[520];

	bool isVisible(Player* localPlayer)
	{
		DWORD traceLine = 0x048A310;
		Vector4 traceresult;
		traceresult.w = 0;
		Vector3 src = localPlayer->headPosition;
		Vector3 dst = headPosition;

		__asm
		{
			push 0; bSkipTags
			push 0; bCheckPlayers
			push localPlayer
			push dst.z
			push dst.y
			push dst.x
			push src.z
			push src.y
			push src.x
			lea eax, [traceresult]
			call traceLine;
			add esp, 36
		}
		return !traceresult.w;

	}
}; //Size: 0x0440

struct PlayersList
{
	Player* ents[31];
};


class VTable
{
public:
	void* Destructor; //0x0000
	char pad_0004[64]; //0x0004
}; //Size: 0x0044
static_assert(sizeof(VTable) == 0x44);

class Ammo
{
public:
	int32_t ammoOutside; //0x0000
	char pad_0004[36]; //0x0004
	int32_t ammoInside; //0x0028
	char pad_002C[76]; //0x002C
	int32_t nbShotsFired; //0x0078
	char pad_007C[200]; //0x007C
}; //Size: 0x0144
static_assert(sizeof(Ammo) == 0x144);

class Ammo2
{
public:
	int32_t ammoInside; //0x0000
	char pad_0004[36]; //0x0004
	int32_t shootAndReloadCD; //0x0028
	char pad_002C[12]; //0x002C
	int32_t maxAmmoOutside; //0x0038
	char pad_003C[264]; //0x003C
}; //Size: 0x0144
static_assert(sizeof(Ammo2) == 0x144);