#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"

extern Texture2D copperTexture;
extern Texture2D copperVeinTexture;
extern Texture2D ironTexture;
extern Texture2D ironVeinTexture;
extern Texture2D coalTexture;
extern Texture2D coalVeinTexture;
extern Texture2D rockVeinTexture;
extern Texture2D rockTexture;
extern Texture2D steelTexture;

extern Texture2D sandVeinTexture;
extern Texture2D waterVeinTexture;
extern Texture2D oilVeinTexture;

extern Texture2D copperLingotTexture;
extern Texture2D ironLingotTexture;
extern Texture2D copperPlateTexture;
extern Texture2D copperRodTexture;
extern Texture2D ironPlateTexture;
extern Texture2D ironRodTexture;

extern Texture2D defaultTexture;

extern Texture2D drillTexture;
extern Texture2D purpleTexture;
extern Texture2D conveyorTexture;
extern Texture2D pipeTexture;
extern Texture2D chestTexture;
extern Texture2D furnaceTexture;
extern Texture2D solarpanelTexture;
extern Texture2D steamcentralTexture;
extern Texture2D piloneTexture;
extern Texture2D batteryTexture;
extern Texture2D piloneEffectTexture;
extern Texture2D pressTexture;
extern Texture2D stretchTexture;

extern Texture2D gearTexture;

extern Texture2D copperCableTexture;

void InitTexture();
void UnloadAllTexture();

#endif