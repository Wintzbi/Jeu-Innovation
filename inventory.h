#ifndef INVENTORY_H
#define INVENTORY_H
#include "raylib.h"
#include "texture.h"
#include <string.h> // Pour utiliser strcmp
#define INVENTORY_SIZE 10

typedef struct Item {
    char name[20];
    Texture2D texture;
    int quantity;
} Item;

// Déclarations externes
extern int selectedItem; // Déclaration externe de selectedItem
extern bool itemFound;   // Déclaration externe de itemFound
extern Item inventory[INVENTORY_SIZE]; // Déclaration externe de inventory
extern bool isInventoryOpen;
extern bool isCaseClicked;
extern bool TextureFollow;
extern bool RightClick;
extern int indTexture;
extern int  NewindTexture;

void DrawInventoryBar();
void InitInventory();
void DrawInventoryPage();
void DrawInventorySlot();
void FollowMouse(int x,int y,int z);
int checkPos(int x,int y);


#endif