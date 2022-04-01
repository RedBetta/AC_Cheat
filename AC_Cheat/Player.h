#pragma once
#include <stdint.h>

#include "Vector.h"



// Created with ReClass.NET 1.2 by KN4CK3R

class Player
{
public:
	char pad_0000[4];
	Vector3 headPosition; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 bodyPosition; //0x0034
	char pad_0040[40]; //0x0040
	int32_t isJumping; //0x0068
	char pad_006C[4]; //0x006C
	int32_t isMoving; //0x0070
	char pad_0074[132]; //0x0074
	int32_t healthGUI; //0x00F8
	int32_t armor; //0x00FC
	char pad_0100[12]; //0x0100
	int32_t pistolAmmoOutsideClip; //0x010C
	char pad_0110[24]; //0x0110
	int32_t mainWeaponAmmoOutsideClip; //0x0128
	char pad_012C[16]; //0x012C
	int32_t pistolAmmoInClip; //0x013C
	char pad_0140[16]; //0x0140
	int32_t mainWeaponAmmoInClip; //0x0150
	char pad_0154[4]; //0x0154
	int32_t nbOfGrenades; //0x0158
	char pad_015C[4]; //0x015C
	int32_t KnifeBlowCooldown; //0x0160
	int32_t pistolFireCooldown; //0x0164
	char pad_0168[16]; //0x0168
	int32_t mainWeaponFireCooldown; //0x0178
	char pad_017C[4]; //0x017C
	int32_t grenadeCooldown; //0x0180
	char pad_0184[4]; //0x0184
	int32_t nbOfKnifeBlows; //0x0188
	int32_t nbOfPistolFiredShots; //0x018C
	char pad_0190[16]; //0x0190
	int32_t nbOfMainWeaponFiredShots; //0x01A0
	char pad_01A4[128]; //0x01A4
	int8_t bAttack; //0x0224
	char pad_0225[263]; //0x0225
	uint8_t team; //0x032C
	char pad_032D[11]; //0x032D
	uint8_t state; //0x0338
	char pad_0339[59]; //0x0339
	class weapon* currentWeapon; //0x0374
	char pad_0378[200]; //0x0378


	//struct traceresult_s
	//{
	//	Vector3 end;
	//	bool collided;
	//};


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

class weapon
{
public:
	char pad_0000[8]; //0x0000
	class GunOWNER* owner; //0x0008
	class GunINFO* gunInfo; // 0x00C
	char pad_000C[4]; //0x0010
	class ammoclip* ammoPtr; //0x0014
	char pad_0018[4];
	int64_t shotsWithoutInterruption;
	char pad_0020[44]; //0x0018
}; //Size: 0x0044


class GunINFO
{
public:
	char pad_0000[290]; // 0x0000
	int16_t recoilMagicNumber;

};

class GunOWNER
{
public:
	char pad_0000[16]; //0x0000
	Vector3 directionVelocityVector;
}; //Size: 0x0044

class N0000044A
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N0000044A) == 0x44);

class ammoclip
{
public:
	int32_t ammo; //0x0000
	char pad_0004[36]; //0x0004
	int32_t fireCoolDown; //0x0028
	char pad_002C[24]; //0x002C
}; //Size: 0x0044
static_assert(sizeof(ammoclip) == 0x44);