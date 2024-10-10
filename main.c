#include "global.h"

int main(void) {

    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");
    // Charger les textures des items
    InitTexture();
    InitGrid();

    // Initialisation de l'inventaire avec des textures et des quantités d'exemple
    InitInventory();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)
        InitInventoryKeyBiding();

        // Placer un bloc avec un clic droit
        rightClic();
        leftClic();

        // Début du dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner la grille
        GridDraw();

        mouseDefault();

        // Dessiner l'inventaire
        DrawInventory();
        EndDrawing();
    }

    // Déchargement des textures
    UnloadAllTexture();

    CloseWindow();

    return 0;
}