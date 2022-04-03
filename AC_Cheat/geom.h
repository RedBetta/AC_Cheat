#pragma once
#include <algorithm> // sqrt...
#include "Vector.h"


bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight);
float getMagnitude(Vector3 v1, Vector3 v2);
float calcYaw(Vector3 localPlayerPos, Vector3 otherPlayerPos);
float calcPitch(Vector3 localPlayerPos, Vector3 otherPlayerPos);
