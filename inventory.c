#include "cell.h"
#include "inventory.h"
#include "musique.h"

bool itemFound = false;         // Définition de itemFound
int selectedItem = 0;          // Définition de l'index de l'item sélectionné
Item inventory[INVENTORY_SIZE]; // Inventaire avec 10 types de blocs
bool isInventoryOpen = false;
bool isCaseClicked=false;

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
    inventory[20] = (Item) {coalTexture, 10};
}

// Fonction pour dessiner l'inventaire avec des images agrandies
void DrawInventoryBar() {
    int invWidth = screenWidth;
    int invHeight = 50;  // Fixed height for inventory
    int slotSize = invWidth / (10);  // Size of each inventory slot

    // Size of textures in the inventory
    int targetImageSize = 32;

    for (int i = 0; i <= (10); i++) {
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

void DrawInventorySlot() {
    int invWidth = screenWidth;
    int rect_per_line=10;
    int slotSize = (invWidth-20) / (rect_per_line);  // Size of each inventory slot

    for(int j=0;j<2;j++){
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            // Draw the inventory slot with DARKGRAY
            Color slotColor = Fade(DARKGRAY, 0.9f);
            DrawRectangle(((i % rect_per_line) * slotSize)+10, 200 + (i / rect_per_line) * slotSize,slotSize, slotSize, slotColor);
            DrawRectangleLines(((i % rect_per_line) * slotSize)+10, 200 + slotSize*(i / rect_per_line), slotSize, slotSize, BLACK);

            // Highlight the selected slot
            if (i == selectedItem) {
                Rectangle borderRect = { (float)(i * slotSize)+10, 200, (float)slotSize, (float)slotSize };
                DrawRectangleLinesEx(borderRect, 4, Fade(LIGHTGRAY, 0.8f));  // Draw border
            }

            // Check if the quantity is > 0 to draw the enlarged texture
            if (inventory[i].quantity > 0) {
                Rectangle source = { 0, 0, inventory[i].texture.width, inventory[i].texture.height };
                Rectangle dest = { ((i % rect_per_line) * slotSize)+10, 195 + slotSize*(i / rect_per_line),slotSize, slotSize };
                Vector2 origin = { 0, 0 };
                DrawTexturePro(inventory[i].texture, source, dest, origin, 0.0f, WHITE);
                DrawText(TextFormat("%d", inventory[i].quantity), ((i % rect_per_line) * slotSize)+10, 195 + slotSize*(i / rect_per_line), 20, WHITE); // Display quantity
                itemFound = true;
            } else {
                itemFound = false;
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        isCaseClicked=!isCaseClicked;
        if (isCaseClicked) FollowMouse(slotSize);
    }
    

}

void DrawInventoryPage(){
    DrawRectangle(100, 10, screenWidth-200, 60, DARKGRAY);
    DrawText(TextFormat("Inventaire"), 330, 11, 60, WHITE); // Display quantity
    DrawInventorySlot();

}
void FollowMouse(int slotSize) {
    // Obtenir la position de la souris
    Vector2 mousePosition = GetMousePosition();
    
    // Calculer la position en fonction de la grille (si nécessaire)
    int posX = (int)(mousePosition.x / slotSize);
    int posY = (int)(mousePosition.y / slotSize);

    // Dessiner un rectangle à la position de la souris (suivant la grille)
    DrawRectangle(posX * slotSize +10, posY * slotSize +10, slotSize, slotSize, Fade(RED, 0.3f));

    // Définir la source de la texture à afficher (tout le sprite)
    Rectangle source = (Rectangle){ 0, 0, inventory[0].texture.width, inventory[0].texture.height };

    // Définir la destination de la texture (la position où elle sera dessinée)
    Rectangle dest = (Rectangle){ mousePosition.x - slotSize / 2, 
                                  mousePosition.y - slotSize / 2, 
                                  slotSize, 
                                  slotSize };

    // L'origine de la texture est son centre (pour la suivre précisément)
    Vector2 origin = (Vector2){ inventory[0].texture.width / 2, inventory[0].texture.height / 2 };

    // Dessiner la texture qui suit la souris
    DrawTexturePro(inventory[0].texture, source, dest, origin, 0.0f, WHITE);
}

