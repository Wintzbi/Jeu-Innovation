#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "inventory.h"







int main(void) {

    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");

    // Charger les textures des items
    Texture2D copperTexture = LoadTexture("copper_item.png");
    Texture2D ironTexture = LoadTexture("iron_item.png");
    Texture2D coalTexture = LoadTexture("coal_item.png");

    // Initialisation de la machine et des couleurs
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            grid[i][j] = (Cell) {
                .i = i,
                .j = j,
                .placed = false,
                .texture = copperTexture  // Par défaut, on peut utiliser n'importe quelle texture
            };
        }
    }

    // Initialisation de l'inventaire avec des textures et des quantités d'exemple
    inventory[0] = (Item) { copperTexture, 2 };  // Bloc de cuivre avec 2 unités
    inventory[1] = (Item) { ironTexture, 2 };    // Bloc de fer avec 2 unités
    inventory[2] = (Item) { coalTexture, 20 };   // Bloc de charbon avec 20 unités
    inventory[3] = (Item) {};
    inventory[4] = (Item) {};
    inventory[5] = (Item) {};
    inventory[6] = (Item) {};
    inventory[7] = (Item) {};
    inventory[8] = (Item) {};
    inventory[9] = (Item) {};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        int posX = (int)(GetMousePosition().x / cellSize);
        int posY = (int)(GetMousePosition().y / cellSize);

        // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)
        if (IsKeyPressed(KEY_ONE)) selectedItem = 0;
        if (IsKeyPressed(KEY_TWO)) selectedItem = 1;
        if (IsKeyPressed(KEY_THREE)) selectedItem = 2;
        if (IsKeyPressed(KEY_FOUR)) selectedItem = 3;
        if (IsKeyPressed(KEY_FIVE)) selectedItem = 4;
        if (IsKeyPressed(KEY_SIX)) selectedItem = 5;
        if (IsKeyPressed(KEY_SEVEN)) selectedItem = 6;
        if (IsKeyPressed(KEY_EIGHT)) selectedItem = 7;
        if (IsKeyPressed(KEY_NINE)) selectedItem = 8;
        if (IsKeyPressed(KEY_ZERO)) selectedItem = 9;

        // Placer un bloc avec un clic droit
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && inventory[selectedItem].quantity > 0) {
            if (IndexIsValid(posX, posY) && !grid[posX][posY].placed) {
                grid[posX][posY].placed = true;
                grid[posX][posY].texture = inventory[selectedItem].texture;
                inventory[selectedItem].quantity--;  // Décrémenter la quantité

                // Si la quantité atteint 0, réinitialiser la case de l'inventaire
                if (inventory[selectedItem].quantity == 0) {
                    inventory[selectedItem].texture = (Texture2D){ 0 };  // Réinitialiser la texture
                }
            }
        }


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (IndexIsValid(posX, posY) && grid[posX][posY].placed) {
                Texture2D brokenTexture = grid[posX][posY].texture;  // Texture du bloc cassé
                bool itemFoundInInventory = false;

                // Chercher l'item correspondant dans l'inventaire
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i].texture.id == brokenTexture.id) {
                        inventory[i].quantity++;  // Incrémenter la quantité dans l'inventaire
                        itemFoundInInventory = true;  // L'item est trouvé dans l'inventaire
                        break;
                    }
                }

                // Si l'item n'est pas dans l'inventaire, l'ajouter dans la première case disponible
                if (!itemFoundInInventory) {
                    for (int i = 0; i < INVENTORY_SIZE; i++) {
                        if (inventory[i].quantity == 0) {  // Case disponible
                            inventory[i].texture = brokenTexture;  // Ajouter la texture du bloc
                            inventory[i].quantity = 1;  // Initialiser avec 1 bloc
                            break;
                        }
                    }
                }

                // Retirer le bloc de la grille
                grid[posX][posY].placed = false;
            }
        }


        // Début du dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner la grille
        for (int i = 0; i < COL; i++) {
            for (int j = 0; j < ROW; j++) {
                CellDraw(grid[i][j]);
            }
        }

        DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(RED, 0.3f));

        // Dessiner l'inventaire
        DrawInventory();
        EndDrawing();
    }

    // Déchargement des textures
    UnloadTexture(copperTexture);
    UnloadTexture(ironTexture);
    UnloadTexture(coalTexture);

    CloseWindow();

    return 0;
}
