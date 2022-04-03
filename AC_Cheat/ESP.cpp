#include "pch.h"
#include "ESP.h"


bool ESP::IsTeamGame()
{
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
bool ESP::IsEnemy(Player* e)
{
	if (localPlayer->team == e->team)
		return false;
	else return true;
}
bool ESP::isValidEnt(Player* ent)
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

void ESP::DrawESPBox(Player* e, Vector2 bodyScreen, Vector2 headScreen, GL::Font& font)
{
	const GLubyte* color = nullptr;
	if (IsTeamGame() && !IsEnemy(e))
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
	glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport (width and height of the screen)
	for (int i = 0; i < *numOfPlayers; i++)
	{
		if (isValidEnt(entlist->ents[i]))
		{
			Player* e = entlist->ents[i];

			Vector2 bodyScreenCoords;
			Vector2 headScreenCoords;

			if (WorldToScreen(e->bodyPosition, bodyScreenCoords, matrix, viewport[2], viewport[3]))
			{
				if (WorldToScreen(e->headPosition, headScreenCoords, matrix, viewport[2], viewport[3]))
				{
					DrawESPBox(e, bodyScreenCoords, headScreenCoords, font);
				}
				
			}

		}
	}
}