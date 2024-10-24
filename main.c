#include "global.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");   // Initialisation de la fenêtre

    InitTexture();  // Charger les textures des items

    InitGrid();  // Initialiser la grille
    InitMusic();
    InitInventory();  // Initialiser l'inventaire avec des textures et des quantités d'exemple
    InitBaseCraft();
    SetTargetFPS(60);  // Définir la fréquence d'images cible à 60 FPS

    GameScreen currentScreen = MENU;  // État initial du jeu : MENU

    bool isInventoryOpen = false;  // Inventaire fermé par défaut
    bool isOptionOpen=false;
    bool isCraftOpen=false;

    ButtonPlay();  // Initialiser le bouton Play
    while (!WindowShouldClose()) {
        UpdateMusic();
        InitInventoryKeyBiding();  // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)

        BeginDrawing();
        ClearBackground(RAYWHITE);  // Effacer l'écran avec un fond blanc

        if (currentScreen == MENU) {
            DrawText("Minc Corp Simulation", screenWidth / 2 - 150, screenHeight / 2 - 100, 30, DARKGRAY);  // Afficher le titre du menu

            if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = GAME;  // Passer à l'état GAME si le bouton Play est cliqué
                }
                DrawRectangleRec(playButton, LIGHTGRAY);  // Afficher le bouton "Play" survolé
            } else {
                DrawRectangleRec(playButton, GRAY);  // Afficher le bouton "Play" normal
            }
            DrawText("Play", screenWidth / 2 - 20, screenHeight / 2 - 10, 20, BLACK);  // Afficher le texte "Play" sur le bouton
            DrawMenu(&currentScreen);  // Dessiner le menu
        }

        if (IsKeyPressed(KEY_E)) {
            isInventoryOpen = !isInventoryOpen;  // Ouvrir/fermer l'inventaire avec la touche E

            if (!isInventoryOpen) {
                currentScreen = GAME;  // Revenir à l'état GAME si l'inventaire est fermé
            }
        }
        if (IsKeyPressed(KEY_TAB)) {
            isOptionOpen = !isOptionOpen;  // Ouvrir/fermer l'inventaire avec la touche E

            if (!isOptionOpen) {
                currentScreen = GAME;  // Revenir à l'état GAME si l'inventaire est fermé
            }
        }

        if (IsKeyPressed(KEY_C)) {
                isCraftOpen = !isCraftOpen;  // Ouvrir/fermer l'inventaire avec la touche E

                if (!isCraftOpen) {
                    currentScreen = GAME;  // Revenir à l'état GAME si l'inventaire est fermé
                }
            }

        if (isInventoryOpen) {
            currentScreen = INVENT;  // Passer à l'état INVENT si l'inventaire est ouvert
            DrawInventoryPage(); 
            } 
            

        else if (isOptionOpen) {
            currentScreen = OPTION;  // Passer à l'état ESCAPE si l'inventaire est ouvert
            DrawEscapePage();

        } 

        else if (isCraftOpen) {
            currentScreen = CRAFT;  // Passer à l'état ESCAPE si l'inventaire est ouvert
            DrawCraftPage();
        } 
        else if (currentScreen == GAME) {
            GridDraw();  // Dessiner la grille de jeu

            rightClic();  // Placer un bloc avec un clic droit

            leftClic();  // Récupérer un bloc avec un clic gauche

            mouseDefault();  // Afficher la souris par défaut

            DrawInventoryBar();  // Dessiner la barre de l'inventaire
        }

        EndDrawing();  // Terminer le dessin de la frame
    }
    UnloadMusic();
    UnloadAllTexture();  // Décharger les textures
    CloseWindow();  // Fermer la fenêtre
    return 0;
}