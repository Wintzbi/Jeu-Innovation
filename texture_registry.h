#ifndef TEXTURE_REGISTRY_H
#define TEXTURE_REGISTRY_H

#include "raylib.h"

// Indices logiques stables — l'ordre correspond à InitTexture() dans texture.c
// 0 est réservé pour "pas de texture" ({0})
typedef enum TextureID {
    TEX_NONE = 0,
    TEX_COPPER,
    TEX_IRON,
    TEX_COAL,
    TEX_COAL_VEIN,
    TEX_ROCK_VEIN,
    TEX_ROCK,
    TEX_STEEL,
    TEX_GEAR,
    TEX_SAND_VEIN,
    TEX_WATER_VEIN,
    TEX_OIL_VEIN,
    TEX_COPPER_LINGOT,
    TEX_IRON_LINGOT,
    TEX_COPPER_PLATE,
    TEX_COPPER_ROD,
    TEX_IRON_PLATE,
    TEX_IRON_ROD,
    TEX_COPPER_VEIN,
    TEX_IRON_VEIN,
    TEX_COPPER_CABLE,
    TEX_SAND_DUST,
    TEX_DEFAULT,
    TEX_DRILL,
    TEX_CONVEYOR,
    TEX_PIPE,
    TEX_CHEST,
    TEX_FURNACE,
    TEX_PURPLE,
    TEX_SOLAR_PANEL,
    TEX_STEAM_CENTRAL,
    TEX_PILONE,
    TEX_PILONE_EFFECT,
    TEX_BATTERY,
    TEX_PRESS,
    TEX_STRETCH,
    TEX_LANTERN,
    TEX_STEAM_EFFECT,
    TEX_FURNACE_EFFECT,
    TEX_PRESS_EFFECT,
    TEX_STRETCH_EFFECT,
    TEX_COUNT  // toujours en dernier
} TextureID;

// Construit le registre — appeler après InitTexture()
void InitTextureRegistry(void);

// Texture → index logique stable (retourne TEX_NONE si inconnue)
int TexToIdx(Texture2D tex);

// Index logique → Texture2D (retourne {0} pour TEX_NONE)
Texture2D IdxToTex(int idx);

#endif
