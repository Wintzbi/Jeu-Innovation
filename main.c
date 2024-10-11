#include "global.h"

int main(void) {

    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");
    // Charger les textures des items
    InitTexture();

    // Inititaliser la grille
    InitGrid();

    // Initialisation de l'inventaire avec des textures et des quantités d'exemple
    InitInventory();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)
        InitInventoryKeyBiding();

        // Placer un bloc avec un clic droit
        rightClic();
        // Le récupérer grâce au clil gauche
        leftClic();

        // Début du dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner la grille
        if (IsKeyPressed(KEY_E))
        {
            isInventoryOpen = !isInventoryOpen;  // Inverser l'état de l'inventaire
        }
        if (!isInventoryOpen)
        {
            GridDraw();
        }
        

        // Afficher la souris par défault
        mouseDefault();

        // Dessiner l'inventaire
        DrawInventoryBar();
        EndDrawing();
    }

    // Déchargement des textures
    UnloadAllTexture();
    CloseWindow();

    return 0;
}