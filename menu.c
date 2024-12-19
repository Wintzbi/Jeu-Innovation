#include "menu.h"
#include "cell.h"

// Variable pour le bouton Play
Rectangle playButton,LargeButton,LilButton;

// Fonction pour initialiser le bouton Play
void ButtonPlay() {
    playButton = (Rectangle) {screenWidth / 2 - 50, screenHeight / 2 - 25, 100, 50 };
    LargeButton = (Rectangle) {screenWidth / 2 - 100, screenHeight / 2 + 50, 200, 50 }; 
    LilButton = (Rectangle) {screenWidth / 2 - 300, screenHeight / 2 + 50, 100, 50 };
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
        if (CheckCollisionPointRec(GetMousePosition(), LargeButton)) {
        DrawRectangleRec(LargeButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            screenHeight = 1080;
            screenWidth = 1920;
            CloseWindow();
            InitWindow(screenWidth, screenHeight, "Minc Corp simulation");
            ButtonPlay();
        }
    } else {
        DrawRectangleRec(LargeButton, GRAY);  // Couleur normale
    }
    DrawText(TextFormat("1920x1080"), LargeButton.x + 10, LargeButton.y + (LargeButton.height / 2) - 10, 19, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), LilButton)) {
        DrawRectangleRec(LilButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            screenHeight = 1200 * 0.75;
            screenWidth = 1920 * 0.75;
            CloseWindow();
            InitWindow(screenWidth, screenHeight, "Minc Corp simulation");
            ButtonPlay();
        }
    } else {
        DrawRectangleRec(LilButton, GRAY);  // Couleur normale
    }
    DrawText(TextFormat("1440x900"), LilButton.x + 10, LilButton.y + (LilButton.height / 2) - 10, 19, WHITE);
}
