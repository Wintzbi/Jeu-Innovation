#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

extern Camera2D camera;
extern Camera2D miniMapCamera;
void setPlayerCamera();
void moveCamera();
void DrawMiniMap();

#endif
