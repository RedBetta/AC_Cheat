#pragma once
#include <Windows.h>
//These two function are from a old fleep post, i was able to use them to create the box style however this was just to save time :) Thx Fleep
void DrawFilledRect(LONG x, LONG y, LONG w, LONG h, HBRUSH brush, HDC hdc)
{
    RECT rect = { x, y, x + w, y + h };
    FillRect(hdc, &rect, brush);
}


void DrawBorderBox(LONG x, LONG y, LONG w, LONG h, LONG thickness, HBRUSH brush, HDC hdc)
{

    DrawFilledRect(x, y, w, thickness, brush, hdc); // bottom 

    DrawFilledRect(x, y, thickness, h, brush, hdc); // right

    DrawFilledRect((x + w), y, thickness, h, brush, hdc);

    DrawFilledRect(x, y + h, w + thickness, thickness, brush, hdc);
}

void DrawLine(int targetX, int targetY, HDC hdc)
{
    MoveToEx(hdc, 960, 1080, NULL);
    LineTo(hdc, targetX, targetY);

}

void DrawESP(Vector2 &bodyPosOnScreen, Vector2 &headPosOnScreen, HDC hdc, HBRUSH brush)
{
    //Creates the head height
    float head = headPosOnScreen.y - bodyPosOnScreen.y;
    float width = head / 2;
    float center = width / -2;
    //Creates Extra area above head
    float extra = head / -6;

    //Draws the box
    DrawBorderBox((LONG)(bodyPosOnScreen.x + center), (LONG)(bodyPosOnScreen.y), (LONG)width, (LONG)(head - extra), 1, brush, hdc);
}