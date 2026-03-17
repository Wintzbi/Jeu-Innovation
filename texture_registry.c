#include "texture_registry.h"
#include "texture.h"

// Tableau indexé par TextureID — rempli dans InitTextureRegistry()
static Texture2D registry[TEX_COUNT];

void InitTextureRegistry(void) {
    registry[TEX_NONE]           = (Texture2D){0};
    registry[TEX_COPPER]         = copperTexture;
    registry[TEX_IRON]           = ironTexture;
    registry[TEX_COAL]           = coalTexture;
    registry[TEX_COAL_VEIN]      = coalVeinTexture;
    registry[TEX_ROCK_VEIN]      = rockVeinTexture;
    registry[TEX_ROCK]           = rockTexture;
    registry[TEX_STEEL]          = steelTexture;
    registry[TEX_GEAR]           = gearTexture;
    registry[TEX_SAND_VEIN]      = sandVeinTexture;
    registry[TEX_WATER_VEIN]     = waterVeinTexture;
    registry[TEX_OIL_VEIN]       = oilVeinTexture;
    registry[TEX_COPPER_LINGOT]  = copperLingotTexture;
    registry[TEX_IRON_LINGOT]    = ironLingotTexture;
    registry[TEX_COPPER_PLATE]   = copperPlateTexture;
    registry[TEX_COPPER_ROD]     = copperRodTexture;
    registry[TEX_IRON_PLATE]     = ironPlateTexture;
    registry[TEX_IRON_ROD]       = ironRodTexture;
    registry[TEX_COPPER_VEIN]    = copperVeinTexture;
    registry[TEX_IRON_VEIN]      = ironVeinTexture;
    registry[TEX_COPPER_CABLE]   = copperCableTexture;
    registry[TEX_SAND_DUST]      = sandDust;
    registry[TEX_DEFAULT]        = defaultTexture;
    registry[TEX_DRILL]          = drillTexture;
    registry[TEX_CONVEYOR]       = conveyorTexture;
    registry[TEX_PIPE]           = pipeTexture;
    registry[TEX_CHEST]          = chestTexture;
    registry[TEX_FURNACE]        = furnaceTexture;
    registry[TEX_PURPLE]         = purpleTexture;
    registry[TEX_SOLAR_PANEL]    = solarpanelTexture;
    registry[TEX_STEAM_CENTRAL]  = steamcentralTexture;
    registry[TEX_PILONE]         = piloneTexture;
    registry[TEX_PILONE_EFFECT]  = piloneEffectTexture;
    registry[TEX_BATTERY]        = batteryTexture;
    registry[TEX_PRESS]          = pressTexture;
    registry[TEX_STRETCH]        = stretchTexture;
    registry[TEX_LANTERN]        = lanternTexture;
    registry[TEX_STEAM_EFFECT]   = steamEffect;
    registry[TEX_FURNACE_EFFECT] = furnaceEffect;
    registry[TEX_PRESS_EFFECT]   = pressEffect;
    registry[TEX_STRETCH_EFFECT] = stretchEffect;
}

int TexToIdx(Texture2D tex) {
    if (tex.id == 0) return TEX_NONE;
    for (int i = 1; i < TEX_COUNT; i++)
        if (registry[i].id == tex.id) return i;
    return TEX_NONE;  // texture inconnue → ignorée
}

Texture2D IdxToTex(int idx) {
    if (idx <= TEX_NONE || idx >= TEX_COUNT)
        return (Texture2D){0};
    return registry[idx];
}
