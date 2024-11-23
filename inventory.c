#include "cell.h"
#include "inventory.h"
#include "musique.h"

bool itemFound = false;         // Définition de itemFound
int selectedItem = 0;          // Définition de l'index de l'item sélectionné
Item inventory[INVENTORY_SIZE]; // Inventaire avec 10 types de blocs
bool isInventoryOpen = false;
bool isCaseClicked=false;
bool TextureFollow=false;
int indTexture=-1;
int NewindTexture=-1;
bool RightClick=false;


void InitInventory() {
    inventory[0] = (Item) { "Cuivre",copperTexture, 20 };  // Bloc de cuivre avec 2 unités
    inventory[1] = (Item) { "Fer",ironTexture, 20 };    // Bloc de fer avec 2 unités
    inventory[2] = (Item) { "Charbon",coalTexture, 40 };   // Bloc de charbon avec 20 unités
    inventory[3] = (Item) {"Foreuse",drillTexture, 5};
    inventory[4] = (Item) {"Tapis",conveyorTexture,100};
    inventory[5] = (Item) {"Coffre",chestTexture,10};
    inventory[6] = (Item) {"Furnace",purpleTexture,10};
    inventory[7] = (Item) {"Tuyau",conveyorTexture,100};
    inventory[8] = (Item) {};
    inventory[9] = (Item) {"Charbon",coalTexture, 10};
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
        if (inventory[i].quantity > 0 ) {
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
                if(i!=indTexture || RightClick ){
                Rectangle source = { 0, 0, inventory[i].texture.width, inventory[i].texture.height };
                Rectangle dest = { ((i % rect_per_line) * slotSize)+10, 195 + slotSize*(i / rect_per_line),slotSize, slotSize };
                Vector2 origin = { 0, 0 };
                DrawTexturePro(inventory[i].texture, source, dest, origin, 0.0f, WHITE);
                DrawText(TextFormat("%d", inventory[i].quantity), ((i % rect_per_line) * slotSize)+10, 180+slotSize + slotSize*(i / rect_per_line), 20, WHITE); // Display quantity
                itemFound = true;}
            } else {
                itemFound = false;
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        isCaseClicked=!isCaseClicked;
       
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        isCaseClicked=!isCaseClicked;
        RightClick=true;
       
    }
     if (isCaseClicked) {
        if (!TextureFollow) indTexture= checkPos( slotSize, rect_per_line);
        if(indTexture !=-1){
            if ((RightClick && inventory[indTexture].quantity>1)|| !RightClick) FollowMouse(slotSize,rect_per_line,indTexture);
        }
        
     }

    else if (!isCaseClicked) {
        if (TextureFollow) {
            NewindTexture = checkPos(slotSize, rect_per_line);

            // Vérifier si la nouvelle position n'est pas la même que l'ancienne et qu'elle est valide
            if (NewindTexture != -1 && NewindTexture != indTexture) {
                // Si la case contient déjà un objet du même type (même texture), additionner les quantités
                if (strcmp(inventory[NewindTexture].name, inventory[indTexture].name ) ==0) {
                    inventory[NewindTexture].quantity += inventory[indTexture].quantity;
                    inventory[indTexture] = (Item) {};  // Vider l'ancienne case
                    
                }
                // Si la case est vide, déplacer l'objet
                else if (inventory[NewindTexture].quantity == 0) {
                    inventory[NewindTexture] = inventory[indTexture];
                    if (RightClick) {
                        inventory[NewindTexture].quantity=inventory[indTexture].quantity/2;
                        inventory[indTexture].quantity -= inventory[NewindTexture].quantity;
                    }

                    else inventory[indTexture] = (Item) {};  // Vider l'ancienne case
                    
                }
                indTexture=-1;
                RightClick=false;
                TextureFollow = false;  // Arrêter le suivi de la texture
            } else {
                isCaseClicked = true;  // Rester en mode suivi si la condition de dépôt n'est pas remplie
            }
        }
    }

    

}

void DrawInventoryPage(){
    DrawRectangle(100, 10, screenWidth-200, 60, DARKGRAY);
    DrawText(TextFormat("Inventaire"), 330, 11, 60, WHITE); // Display quantity
    DrawInventorySlot();
    

}

void FollowMouse(int slotSize,int rect_per_line,int indTexture) {
    TextureFollow= true;
    // Obtenez la position de la souris
    Vector2 mousePosition = GetMousePosition();
    
            Rectangle source = { 0, 0, inventory[indTexture].texture.width, inventory[indTexture].texture.height };
            Rectangle dest = { mousePosition.x - slotSize / 2, 
                               mousePosition.y - slotSize / 2, 
                               slotSize, 
                               slotSize };
            Vector2 origin = { inventory[indTexture].texture.width / 2, inventory[indTexture].texture.height / 2 };

            DrawTexturePro(inventory[indTexture].texture, source, dest, origin, 0.0f, WHITE);        
    }

int checkPos(int slotSize,int rect_per_line){
// Obtenez la position de la souris
    Vector2 mousePosition = GetMousePosition();
    
    // Vérifiez si la souris est sur un slot
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int slotX = ((i % rect_per_line) * slotSize) + 10; // Position X du slot
        int slotY = 200 + (i / rect_per_line) * slotSize; // Position Y du slot

        // Vérifiez si la souris est dans les limites du slot
        if (mousePosition.x >= slotX && mousePosition.x <= slotX + slotSize &&
            mousePosition.y >= slotY && mousePosition.y <= slotY + slotSize ){
                return i;
            }
  
}
  return -1;
}