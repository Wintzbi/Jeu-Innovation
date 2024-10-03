#include "raylib.h"

#define INVENTORY_SIZE 10

// Déclarations externes
extern int selectedItem; // Déclaration externe de selectedItem
extern bool itemFound;   // Déclaration externe de itemFound

typedef struct Item {
    Texture2D texture;
    int quantity;
} Item;

extern Item inventory[INVENTORY_SIZE]; // Déclaration externe de inventory

void DrawInventory();