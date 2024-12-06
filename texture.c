#include "raylib.h"
#include "texture.h"

Texture2D copperTexture;
Texture2D copperVeinTexture;
Texture2D ironTexture;
Texture2D ironVeinTexture;
Texture2D coalTexture;
Texture2D coalVeinTexture;
Texture2D rockVeinTexture;
Texture2D rockTexture;

Texture2D sandVeinTexture;
Texture2D waterVeinTexture;
Texture2D oilVeinTexture;

Texture2D copperLingotTexture;
Texture2D ironLingotTexture;
Texture2D copperPlateTexture;
Texture2D copperRodTexture;
Texture2D ironPlateTexture;
Texture2D ironRodTexture;


Texture2D defaultTexture;
Texture2D drillTexture;
Texture2D purpleTexture;
Texture2D conveyorTexture;
Texture2D chestTexture;
Texture2D pipeTexture;
Texture2D furnaceTexture;
Texture2D solarpanelTexture;
Texture2D steamcentralTexture;
Texture2D piloneTexture;
Texture2D piloneEffectTexture;
Texture2D batteryTexture;
Texture2D pressTexture;
Texture2D stretchTexture;

void InitTexture() {
    //ATTENTION ne pas changer l'ordre car les id deffinissent les identifients des itemps
    copperTexture = LoadTexture("assets/copper_item.png");
    ironTexture = LoadTexture("assets/iron_item.png");
    coalTexture = LoadTexture("assets/coal_item.png");
    coalVeinTexture =LoadTexture("assets/coal_vein.png");
    rockVeinTexture=LoadTexture("assets/rockVeinTexture.png");
    rockTexture=LoadTexture("assets/rock_item.png");
    sandVeinTexture =LoadTexture("assets/sand_texture.png");
    waterVeinTexture=LoadTexture("assets/blue_texture.png");
    oilVeinTexture=LoadTexture("assets/purple_texture.png");

    copperLingotTexture = LoadTexture("assets/copper_lingot.png");
    ironLingotTexture = LoadTexture("assets/iron_lingot.png");
    copperPlateTexture =LoadTexture("assets/iron_lingot.png");
    copperRodTexture =LoadTexture("assets/iron_lingot.png");
    ironPlateTexture =LoadTexture("assets/iron_lingot.png");
    ironRodTexture =LoadTexture("assets/iron_lingot.png");



    copperVeinTexture= LoadTexture("assets/copperVeinTexture.png");
    ironVeinTexture= LoadTexture("assets/ironVeinTexture.png");
    defaultTexture = LoadTexture("assets/grass_texture.png");
    drillTexture = LoadTexture("assets/drill.png");
    conveyorTexture= LoadTexture("assets/conveyor.png");
    pipeTexture =LoadTexture("assets/pipe.png");
    chestTexture = LoadTexture("assets/crate.png");
    furnaceTexture=LoadTexture("assets/furnace.png");
    purpleTexture = LoadTexture("assets/purple_texture.png");
    solarpanelTexture = LoadTexture("assets/purple_texture.png");
    steamcentralTexture = LoadTexture("assets/red_texture.png");
    piloneTexture = LoadTexture("assets/coil.png");
    piloneEffectTexture = LoadTexture("assets/coil_effect.png");
    batteryTexture = LoadTexture("assets/blue_texture.png");
    pressTexture = LoadTexture("assets/furnace.png");
    stretchTexture = LoadTexture("assets/furnace.png");
}

void UnloadAllTexture() {
    UnloadTexture(copperTexture);
    UnloadTexture(ironTexture);
    UnloadTexture(coalTexture);
    UnloadTexture(coalVeinTexture);
    UnloadTexture(rockVeinTexture);
    UnloadTexture(rockTexture);

    UnloadTexture(sandVeinTexture);
    UnloadTexture(waterVeinTexture);
    UnloadTexture(oilVeinTexture);

    UnloadTexture(copperLingotTexture);
    UnloadTexture(ironLingotTexture);
    UnloadTexture(copperPlateTexture);
    UnloadTexture(copperRodTexture);
    UnloadTexture(ironPlateTexture);
    UnloadTexture(ironRodTexture);

    UnloadTexture(defaultTexture);
    UnloadTexture(copperVeinTexture);
    UnloadTexture(ironVeinTexture);
    UnloadTexture(drillTexture);
    UnloadTexture(purpleTexture);
    UnloadTexture(conveyorTexture);
    UnloadTexture(pipeTexture);
    UnloadTexture(chestTexture);
    UnloadTexture(furnaceTexture);
    UnloadTexture(solarpanelTexture);
    UnloadTexture(steamcentralTexture);
    UnloadTexture(piloneTexture);
    UnloadTexture(batteryTexture);
    UnloadTexture(piloneEffectTexture);
    UnloadTexture(pressTexture);
    UnloadTexture(stretchTexture);


}