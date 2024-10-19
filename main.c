#include "global.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");

    InitTexture();
    InitGrid();
    InitMusic();
    InitInventory();

    SetTargetFPS(60);

    GameScreen currentScreen = MENU;
    bool isInventoryOpen = false;
    bool isOptionOpen = false;

    ButtonPlay();
    while (!WindowShouldClose()) {
        UpdateMusic();
        InitInventoryKeyBiding();

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

        if (isInventoryOpen) {
            currentScreen = INVENT;
            DrawInventoryPage();
        } else if (isOptionOpen) {
            currentScreen = OPTION;
            DrawEscapePage();
        } else if (currentScreen == GAME) {
            CameraUpdateAndDraw();  // Mise à jour et dessin de la caméra pour déplacer la vue
            rightClic();
            leftClic();
            mouseDefault();
            DrawInventoryBar();
        }

        EndDrawing();
    }

    UnloadMusic();
    UnloadAllTexture();
    CloseWindow();

    return 0;
}
