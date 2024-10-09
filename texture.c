#include "raylib.h"
#include "texture.h"

Texture2D copperTexture;
Texture2D ironTexture;
Texture2D coalTexture;

void InitTexture() {
    copperTexture = LoadTexture("assets/copper_item.png");
    ironTexture = LoadTexture("assets/iron_item.png");
    coalTexture = LoadTexture("assets/coal_item.png");
}

void UnloadAllTexture() {
    UnloadTexture(copperTexture);
    UnloadTexture(ironTexture);
    UnloadTexture(coalTexture);
}
