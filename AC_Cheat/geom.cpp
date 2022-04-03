#include "pch.h"
#include <Windows.h>
#include "geom.h"

// matrix = modelViewMatrix
bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight) // 3D to 2D
{
    //Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
    Vector4 clipCoords{
        pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12],
        pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13],
        pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14],
        pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15]
    };

    // if in us or behind us
    if (clipCoords.w < 0.1f)
        return false;

    //perspective division, dividing by clip.W = Normalized Device Coordinates
    Vector3 NDC{
        clipCoords.x / clipCoords.w,
        clipCoords.y / clipCoords.w,
        clipCoords.z / clipCoords.w
    };

    //Transform to window coordinates
    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}

float getMagnitude(Vector3 v1, Vector3 v2)
{
    return (float)sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2) * 1.0);
}

float calcYaw(Vector3 localPlayerPos, Vector3 otherPlayerPos)
{
    const double pi = 3.14159;
    return (float)atan2(otherPlayerPos.y - localPlayerPos.y, otherPlayerPos.x - localPlayerPos.x) * (float)(180 / pi);
}

float calcPitch(Vector3 localPlayerPos, Vector3 otherPlayerPos)
{
    const double pi = 3.14159;
    float opposite = otherPlayerPos.z - localPlayerPos.z;
    float adjacent = (float)sqrt(pow(otherPlayerPos.y - localPlayerPos.y, 2) + pow(otherPlayerPos.x - localPlayerPos.x, 2));
    return (float)atan2(opposite, adjacent) * (float)(180 / pi);
}