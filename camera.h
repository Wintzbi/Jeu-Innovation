#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

extern Camera2D camera;
void setPlayerCamera();
void moveCamera();
void miniMap(int screenWidth, int screenHeight);

#endif
