#pragma once
#include "glDraw.h"
#include "glText.h"
#include "Player.h"
#include "geom.h"

const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;

class ESP
{
public:
	int* gameMode = (int*)(0x50F49C);
	int* numOfPlayers = (int*)(0x50F500);
	float* matrix = (float*)(0x501AE8);
	Player* localPlayer = *(Player**)0x50F4F4;
	PlayersList* entlist = *(PlayersList**)0x50F4F8;

	int viewport[4];

	bool IsTeamGame();
	bool IsEnemy(Player* e);
	bool isValidEnt(Player* ent);

	void DrawESPBox(Player* e, Vector2 bodyScreen, Vector2 headScreen, GL::Font& font);
	void Draw(GL::Font& font);
};

