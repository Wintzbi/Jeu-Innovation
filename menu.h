#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum GameScreen { MENU = 0, GAME,INVENT,MUSIC,OPTION } GameScreen;

// Déclaration de la variable globale pour le bouton Play
extern Rectangle playButton;

// Fonction pour initialiser le bouton Play
void ButtonPlay();

// Fonction pour gérer l'affichage du menu
void DrawMenu(GameScreen *currentScreen);

#endif
