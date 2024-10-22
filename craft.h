#include "cell.h"
#include "inventory.h"

#define MaxBaseCraft 5

void DrawCraftPage();
void CraftButton();
void DrawCraftMenu();
void DrawBaseCraft();
void InitBaseCraft();

extern bool DropDownMenu;

typedef struct Craft {
    Texture2D texture;
    int quantity;
} Craft;

Craft BaseCraft[MaxBaseCraft];

Rectangle BaseCraftButton, dropdownMenu[MaxBaseCraft];
extern int CraftSelectedItem;
