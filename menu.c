#include "menu.h"
#include "cell.h"

// Variable pour le bouton Play
Rectangle playButton;

// Fonction pour initialiser le bouton Play
void ButtonPlay() {
    playButton = (Rectangle) {screenWidth / 2 - 50, screenHeight / 2 - 25, 100, 50 };
}

// Fonction pour dessiner le menu principal
void DrawMenu(GameScreen *currentScreen) {
    DrawText("Minc Corp Simulation", screenWidth / 2 - 150, screenHeight / 2 - 100, 30, DARKGRAY);

    // Dessiner le bouton "Play"
    if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *currentScreen = GAME;  // Commence le jeu quand le bouton est cliqué
        }
        DrawRectangleRec(playButton, LIGHTGRAY);  // Couleur survolée
    } else {
        DrawRectangleRec(playButton, GRAY);  // Couleur normale
    }

    DrawText("Play", screenWidth / 2 - 20, screenHeight / 2 - 10, 20, BLACK);
}
