#include "cell.h"
#include "inventory.h"

void mouseDefault() {
    int posX = (int)(GetMousePosition().x / cellSize);
    int posY = (int)(GetMousePosition().y / cellSize);
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

    int posX = (int)(GetMousePosition().x / cellSize);
    int posY = (int)(GetMousePosition().y / cellSize);
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

    int posX = (int)(GetMousePosition().x / cellSize);
    int posY = (int)(GetMousePosition().y / cellSize);
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
    
}

