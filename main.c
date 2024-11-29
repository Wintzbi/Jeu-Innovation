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

    const double interval = 1; // Intervalle en secondes
    struct timespec start, current; 
    clock_gettime(CLOCK_MONOTONIC, &start); // Temps de départ
    while (!shouldClose ) {

        clock_gettime(CLOCK_MONOTONIC, &current); // Temps actuel

        double elapsed = (current.tv_sec - start.tv_sec) +
                         (current.tv_nsec - start.tv_nsec) / 1e9;

        if (elapsed >= interval) {
            // Code à exécuter toutes les demi-secondes
            Update_Conv();
            clock_gettime(CLOCK_MONOTONIC, &start); // Réinitialiser le temps de départ
        }

        if (IsKeyPressed(KEY_R)) UpdateDir();
        UpdateMusic();
        InitInventoryKeyBiding();
        UpdateBattery();
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

        if (IsKeyPressed(KEY_ESCAPE)) {
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
        if (IsKeyPressed(KEY_TAB)) {
            selectedItem=(selectedItem+1)%10;
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

            Update_Foreuse();

            Update_Furnace();

            Update_Steam();
            
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
