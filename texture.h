#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"

extern Texture2D copperTexture;
extern Texture2D copperVeinTexture;
extern Texture2D ironVeinTexture;
extern Texture2D ironTexture;
extern Texture2D coalTexture;
extern Texture2D ironLingotTexture;
extern Texture2D copperLingotTexture;
extern Texture2D defaultTexture;

void InitTexture();
void UnloadAllTexture();

#endif