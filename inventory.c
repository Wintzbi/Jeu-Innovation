#include "cell.h"
#include "inventory.h"

bool itemFound = false;         // Définition de itemFound
int selectedItem = 0;          // Définition de l'index de l'item sélectionné
Item inventory[INVENTORY_SIZE]; // Inventaire avec 10 types de blocs
bool isInventoryOpen = false;

void InitInventory() {
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
}

// Fonction pour dessiner l'inventaire avec des images agrandies
void DrawInventory() {
    int invWidth = screenWidth;
    int invHeight = 50;  // Fixed height for inventory
    int slotSize = invWidth / INVENTORY_SIZE;  // Size of each inventory slot

    // Size of textures in the inventory
    int targetImageSize = 32;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        // Draw the inventory slot with DARKGRAY
        Color slotColor = Fade(DARKGRAY, 0.9f);
        DrawRectangle(i * slotSize, 0, slotSize, invHeight, slotColor);
        DrawRectangleLines(i * slotSize, 0, slotSize, invHeight, BLACK);

        // Highlight the selected slot
        if (i == selectedItem) {
            Rectangle borderRect = { (float)(i * slotSize), 0, (float)slotSize, (float)invHeight };
            DrawRectangleLinesEx(borderRect, 4, Fade(LIGHTGRAY, 0.8f));  // Draw border
        }

        // Check if the quantity is > 0 to draw the enlarged texture
        if (inventory[i].quantity > 0) {
            Rectangle source = { 0, 0, inventory[i].texture.width, inventory[i].texture.height };
            Rectangle dest = { i * slotSize + (slotSize - targetImageSize) / 2, 
                               (invHeight - targetImageSize) / 2, 
                               targetImageSize, targetImageSize };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(inventory[i].texture, source, dest, origin, 0.0f, WHITE);
            DrawText(TextFormat("%d", inventory[i].quantity), i * slotSize + 25, 10, 20, WHITE); // Display quantity
            itemFound = true;
        } else {
            itemFound = false;
        }
    }
}