#include "raylib.h"
#include "texture.h"

Texture2D copperTexture;
Texture2D copperVeinTexture;
Texture2D ironTexture;
Texture2D ironVeinTexture;
Texture2D coalTexture;
Texture2D copperLingotTexture;
Texture2D ironLingotTexture;
Texture2D defaultTexture;

void InitTexture() {
    //ATTENTION ne pas changer l'ordre car les id deffinissent les identifients des itemps
    copperTexture = LoadTexture("assets/copper_item.png");
    copperVeinTexture= LoadTexture("assets/copper_vein.png");
    ironVeinTexture= LoadTexture("assets/iron_vein.png");
    ironTexture = LoadTexture("assets/iron_item.png");
    coalTexture = LoadTexture("assets/coal_item.png");
    copperLingotTexture = LoadTexture("assets/copper_lingot.png");
    ironLingotTexture= LoadTexture("assets/iron_lingot.png");
    defaultTexture = LoadTexture("assets/default.png");
}

void UnloadAllTexture() {
    UnloadTexture(copperTexture);
    UnloadTexture(copperVeinTexture);
    UnloadTexture(ironVeinTexture);
    UnloadTexture(ironTexture);
    UnloadTexture(coalTexture);
    UnloadTexture(copperLingotTexture);
    UnloadTexture(ironLingotTexture);
    UnloadTexture(defaultTexture);
}