#include "cell.h"
#include "inventory.h"
#include "camera.h"

void mouseDefault() {
    // Récupérer la position de la souris et la convertir en coordonnées du monde
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);  // Convertir en coordonnées du monde

    // Calculer la position de la cellule sous la souris
    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    // Dessiner une cellule en surbrillance là où la souris est positionnée
    DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(RED, 0.3f));
}


void InitInventoryKeyBiding() {
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
}

void rightClic() {
    // Récupérer la position de la souris et la convertir en coordonnées du monde
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);  // Convertir en coordonnées du monde

    // Calculer la position de la cellule sous la souris
    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

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
}


void leftClic() {
    // Récupérer la position de la souris et la convertir en coordonnées du monde
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);  // Convertir en coordonnées du monde

    // Calculer la position de la cellule sous la souris
    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
<<<<<<< HEAD
            if (IndexIsValid(posX, posY) && grid[posX][posY].placed && grid[posX][posY].pickable) {
                Texture2D brokenTexture = grid[posX][posY].texture;  // Texture du bloc cassé
                bool itemFoundInInventory = false;
=======
        if (IndexIsValid(posX, posY) && grid[posX][posY].placed) {
            Texture2D brokenTexture = grid[posX][posY].texture;  // Texture du bloc cassé
            bool itemFoundInInventory = false;
>>>>>>> grid-camera

            // Chercher l'item correspondant dans l'inventaire
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                if (inventory[i].texture.id == brokenTexture.id) {
                    inventory[i].quantity++;  // Incrémenter la quantité dans l'inventaire
                    itemFoundInInventory = true;
                    break;
                }
            }

            // Si l'item n'est pas dans l'inventaire, l'ajouter dans la première case disponible
            if (!itemFoundInInventory) {
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i].quantity == 0) {  // Case disponible
                        inventory[i].texture = brokenTexture;
                        inventory[i].quantity = 1;
                        break;
                    }
                }
            }

            // Retirer le bloc de la grille
            grid[posX][posY].placed = false;
        }
    }
}





