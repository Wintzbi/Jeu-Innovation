#ifndef INVENTORY_H
#define INVENTORY_H
#include "raylib.h"

#define INVENTORY_SIZE 10
typedef struct Item {
    Texture2D texture;
    int quantity;
} Item;

// Déclarations externes
extern int selectedItem; // Déclaration externe de selectedItem
extern bool itemFound;   // Déclaration externe de itemFound
extern Item inventory[INVENTORY_SIZE]; // Déclaration externe de inventory


void DrawInventory();

#endif
