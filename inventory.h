#ifndef INVENTORY_H
#define INVENTORY_H
#include "raylib.h"
#include "texture.h"

#define INVENTORY_SIZE 40
typedef struct Item {
    Texture2D texture;
    int quantity;
} Item;

// Déclarations externes
extern int selectedItem; // Déclaration externe de selectedItem
extern bool itemFound;   // Déclaration externe de itemFound
extern Item inventory[INVENTORY_SIZE]; // Déclaration externe de inventory
extern bool isInventoryOpen;
extern bool isCaseClicked;

void DrawInventoryBar();
void InitInventory();
void DrawInventoryPage();
void DrawInventorySlot();
void FollowMouse(int x);


#endif