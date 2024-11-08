#include "global.h"

int main(void) {

    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");   // Initialisation de la fenêtre
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

    InitTexture();
    InitGrid();
    InitMusic();

    InitInventory();  // Initialiser l'inventaire avec des textures et des quantités d'exemple
    InitBaseCraft();
    SetTargetFPS(60);  // Définir la fréquence d'images cible à 60 FPS

    setPlayerCamera();

    GameScreen currentScreen = MENU;
    bool isInventoryOpen = false;
    bool isOptionOpen = false;
    bool isCraftOpen=false;

    ButtonPlay();  // Initialiser le bouton Play

    float elapsedTime = 0.0f; // Compteur de temps écoulé

    while (!WindowShouldClose()) {

        
        UpdateMusic();
        InitInventoryKeyBiding();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU) {
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
            DrawCraftPage();
        }

        BeginMode2D(camera); 

        if (currentScreen == GAME) {
            GridDraw();  // Dessiner la grille de jeu
            

            rightClic();  // Placer un bloc avec un clic droit

            leftClic();  // Récupérer un bloc avec un clic gauche

            mouseDefault();  // Afficher la souris par défaut

            moveCamera();
            
        }
        EndMode2D();
        if (currentScreen == GAME) {
        DrawInventoryBar();  // Dessiner la barre de l'inventaire
        }
        EndDrawing();
    }

    UnloadMusic();
    UnloadAllTexture();
    CloseWindow();

    return 0;
}
