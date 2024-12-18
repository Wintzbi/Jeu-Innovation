#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include "raylib.h"

// Constantes pour la vitesse de l'animation
#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

// Structure pour gérer l'animation du sprite
typedef struct SpriteAnimation {
    Texture2D texture;          // Texture du sprite
    Rectangle frameRec;         // Rectangle de la frame actuelle
    Vector2 position;           // Position du sprite à l'écran
    int currentFrame;           // Indice de la frame actuelle
    int framesCounter;          // Compteur de frames pour gérer l'animation
    int framesSpeed;            // Vitesse d'animation (nombre de frames par seconde)
} SpriteAnimation;

// Fonctions pour gérer l'animation du sprite
void InitSpriteAnimation(SpriteAnimation *sprite, const char *fileName, Vector2 position);
void UpdateSpriteAnimation(SpriteAnimation *sprite);
void DrawSpriteAnimation(SpriteAnimation *sprite);
void UnloadSpriteAnimation(SpriteAnimation *sprite);

#endif // SPRITE_ANIMATION_H