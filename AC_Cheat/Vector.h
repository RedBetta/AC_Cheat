#pragma once
#include "constants.h"

struct Vector3
{
    float x, y, z;
};

struct Vector4
{
    float x, y, z, w;
};

struct Vector2
{
    float x, y;
};

float getMagnitude(Vector3 v1, Vector3 v2)
{
    return (float)sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2) * 1.0);
}

float calcYaw(Vector3 localPlayerPos, Vector3 otherPlayerPos)
{
    return (float)atan2(otherPlayerPos.y - localPlayerPos.y, otherPlayerPos.x - localPlayerPos.x) * (float)(180 / pi);
}

float calcPitch(Vector3 localPlayerPos, Vector3 otherPlayerPos)
{
    float opposite = otherPlayerPos.z - localPlayerPos.z;
    float adjacent = (float)sqrt(pow(otherPlayerPos.y - localPlayerPos.y, 2) + pow(otherPlayerPos.x - localPlayerPos.x, 2));
    return (float)atan2(opposite, adjacent) * (float)(180 / pi);

}