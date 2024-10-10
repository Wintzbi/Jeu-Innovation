#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"

extern Texture2D copperTexture;
extern Texture2D ironTexture;
extern Texture2D coalTexture;

void InitTexture();
void UnloadAllTexture();

#endif