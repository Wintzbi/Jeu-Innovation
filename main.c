#include "global.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");

    InitTexture();
    InitGrid();
    InitMusic();

    InitInventory();  // Initialiser l'inventaire avec des textures et des quantités d'exemple
    //InitBaseCraft();
    SetTargetFPS(60);  // Définir la fréquence d'images cible à 60 FPS

    GameScreen currentScreen = MENU;
    bool isInventoryOpen = false;
    bool isOptionOpen = false;
    bool isCraftOpen=false;

    ButtonPlay();

    while (!WindowShouldClose()) {
        UpdateMusic();
        InitInventoryKeyBiding();
        setPlayerCamera();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU) {
            DrawText("Minc Corp Simulation", screenWidth / 2 - 150, screenHeight / 2 - 100, 30, DARKGRAY);

            if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = GAME;
                }
                DrawRectangleRec(playButton, LIGHTGRAY);
            } else {
                DrawRectangleRec(playButton, GRAY);
            }
            DrawText("Play", screenWidth / 2 - 20, screenHeight / 2 - 10, 20, BLACK);
            DrawMenu(&currentScreen);
        }

        if (IsKeyPressed(KEY_E)) {
            isInventoryOpen = !isInventoryOpen;
            if (!isInventoryOpen) {
                currentScreen = GAME;
            }
        }

        if (IsKeyPressed(KEY_TAB)) {
            isOptionOpen = !isOptionOpen;
            if (!isOptionOpen) {
                currentScreen = GAME;
            }
        }

        if (IsKeyPressed(KEY_C)) {
                isCraftOpen = !isCraftOpen;  // Ouvrir/fermer l'inventaire avec la touche E

                if (!isCraftOpen) {
                    currentScreen = GAME;  // Revenir à l'état GAME si l'inventaire est fermé
                }
            }

        if (isInventoryOpen) {
            currentScreen = INVENT;
            DrawInventoryPage();
        } else if (isOptionOpen) {
            currentScreen = OPTION;
            DrawEscapePage();

        } 

        else if (isCraftOpen) {
            currentScreen = CRAFT;  // Passer à l'état ESCAPE si l'inventaire est ouvert
            //DrawCraftPage();
        } 
        else if (currentScreen == GAME) {
            GridDraw();  // Dessiner la grille de jeu

            rightClic();  // Placer un bloc avec un clic droit

            leftClic();  // Récupérer un bloc avec un clic gauche

            mouseDefault();  // Afficher la souris par défaut

            DrawInventoryBar();  // Dessiner la barre de l'inventaire
        }

        EndDrawing();
    }

    UnloadMusic();
    UnloadAllTexture();
    CloseWindow();

    return 0;
}
