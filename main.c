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
        // Commencez le dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU) {
            // Dessiner le menu
            DrawMenu(&currentScreen);
        }
        else if (currentScreen == GAME) {
            // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)
            InitInventoryKeyBiding();

            // Placer un bloc avec un clic droit
            rightClic();

            // Le récupérer grâce au clic gauche
            leftClic();

            // Dessiner la grille
            GridDraw();

            // Afficher la souris par défaut
            mouseDefault();

            // Dessiner l'inventaire
            DrawInventoryBar();
        }

        // Fin du dessin
        EndDrawing();
    }

    // Déchargement des textures
    UnloadAllTexture();
    CloseWindow();

    return 0;
}
