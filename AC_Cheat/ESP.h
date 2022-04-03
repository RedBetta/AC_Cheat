#pragma once
#include "glDraw.h"
#include "glText.h"
#include "Player.h"
#include "geom.h"
#include "assaultcube.h"

const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;

namespace ESP
{
	

	void DrawESPBox(Player* e, Vector2 bodyScreen, Vector2 headScreen, GL::Font& font);
	void Draw(GL::Font& font);
};

