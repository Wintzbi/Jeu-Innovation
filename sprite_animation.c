// sprite_animation.c
#include "sprite_animation.h"

// Initialise l'animation du sprite
void InitSpriteAnimation(SpriteAnimation *sprite, const char *fileName, Vector2 position)
{
    sprite->texture = LoadTexture(fileName);
    sprite->frameRec = (Rectangle){ 0.0f, 0.0f, (float)sprite->texture.width / 6, (float)sprite->texture.height };
    sprite->position = position;
    sprite->currentFrame = 0;
    sprite->framesCounter = 0;
    sprite->framesSpeed = 8;  // Vitesse par défaut
}

// Met à jour l'animation du sprite
void UpdateSpriteAnimation(SpriteAnimation *sprite)
{
    sprite->framesCounter++;

    if (sprite->framesCounter >= (60 / sprite->framesSpeed))
    {
        sprite->framesCounter = 0;
        sprite->currentFrame++;

        if (sprite->currentFrame > 5) sprite->currentFrame = 0;

        sprite->frameRec.x = (float)sprite->currentFrame * sprite->frameRec.width;
    }

    // Contrôle de la vitesse de l'animation
    if (IsKeyPressed(KEY_RIGHT)) sprite->framesSpeed++;
    else if (IsKeyPressed(KEY_LEFT)) sprite->framesSpeed--;

    if (sprite->framesSpeed > MAX_FRAME_SPEED) sprite->framesSpeed = MAX_FRAME_SPEED;
    else if (sprite->framesSpeed < MIN_FRAME_SPEED) sprite->framesSpeed = MIN_FRAME_SPEED;
}

// Dessine l'animation du sprite
void DrawSpriteAnimation(SpriteAnimation *sprite)
{
    DrawTextureRec(sprite->texture, sprite->frameRec, sprite->position, WHITE);
}

// Décharge la texture du sprite
void UnloadSpriteAnimation(SpriteAnimation *sprite)
{
    UnloadTexture(sprite->texture);
}
