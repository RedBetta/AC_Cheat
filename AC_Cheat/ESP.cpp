#include "pch.h"
#include "ESP.h"

void ESP::DrawESPBox(Player* e, Vector2 bodyScreen, Vector2 headScreen, GL::Font& font)
{

	const GLubyte* color = nullptr;
	if (AC::IsTeamGame() && !AC::IsEnemy(e))
	{
		color = rgb::green;
	}
	else
	{
		color = rgb::red;
	}

	float height = bodyScreen.y - headScreen.y;
	height += height / 5;
	float width = height / 2.6f;
	float x = bodyScreen.x - width / 2;
	float y = headScreen.y - height / 5;

	GL::DrawOutline(x, y, width, height, 2.0f, color);

	float textX = font.centerText(x, width, (float)strlen(e->name) * ESP_FONT_WIDTH);
	float textY = y - ESP_FONT_HEIGHT / 2;
	font.Print(textX, textY, color, "%s", e->name);
}


void ESP::Draw(GL::Font& font)
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport (width and height of the screen)
	for (int i = 0; i < *AC::nbPlayers; i++)
	{
		if (AC::isValidEnt(AC::playersList->players[i]))
		{
			Player* e = AC::playersList->players[i];

			Vector2 bodyScreenCoords;
			Vector2 headScreenCoords;

			if (WorldToScreen(e->bodyPosition, bodyScreenCoords, AC::matrix, viewport[2], viewport[3]))
			{
				if (WorldToScreen(e->headPosition, headScreenCoords, AC::matrix, viewport[2], viewport[3]))
				{
					DrawESPBox(e, bodyScreenCoords, headScreenCoords, font);
				}
				
			}

		}
	}
}