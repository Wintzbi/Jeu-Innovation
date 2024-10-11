#include "global.h"
#include "menu.h"

int main(void) {
    // Initialisation de la fenêtre
    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");
    
    // Charger les textures des items
    InitTexture();

    // Initialiser la grille
    InitGrid();

    // Initialisation de l'inventaire avec des textures et des quantités d'exemple
    InitInventory();

    SetTargetFPS(60);

    // État du jeu : MENU ou GAME
    GameScreen currentScreen = MENU;

    // Initialisation du bouton Play
    ButtonPlay();

    while (!WindowShouldClose()) {
        // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)
        InitInventoryKeyBiding();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (currentScreen == MENU) {
            // Menu principal
            DrawText("Minc Corp Simulation", screenWidth / 2 - 150, screenHeight / 2 - 100, 30, DARKGRAY);
            // Dessiner le bouton "Play"
            if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = GAME;  // Commence le jeu quand le bouton est cliqué
                }
                DrawRectangleRec(playButton, LIGHTGRAY);  // Couleur survolée
            } else {
                DrawRectangleRec(playButton, GRAY);  // Couleur normale
            }
            DrawText("Play", screenWidth / 2 - 20, screenHeight / 2 - 10, 20, BLACK);
            // Dessiner le menu
            DrawMenu(&currentScreen);
            
        }
       
        
            // Dessiner la grille
        if (IsKeyPressed(KEY_E))
        {
            isInventoryOpen = !isInventoryOpen;  // Inverser l'état de l'inventaire
        }

        else if (isInventoryOpen) {
            currentScreen = INVENT;
            DrawInventoryPage();
            }

        else if (!isInventoryOpen) currentScreen = GAME;

        if (currentScreen == GAME) {
            GridDraw();
            // Placer un bloc avec un clic droit
            rightClic();
            // Le récupérer grâce au clil gauche
            leftClic();
            // Afficher la souris par défaut
            mouseDefault();

            // Dessiner l'inventaire
            DrawInventoryBar();
        }
              
        

    EndDrawing();
}
    // Déchargement des textures
    UnloadAllTexture();
    CloseWindow();

    return 0;
}
