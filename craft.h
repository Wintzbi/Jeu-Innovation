#ifdef CRAFT_H
#define CRAFT_H
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

extern Craft BaseCraft[MaxBaseCraft];

extern Rectangle BaseCraftButton, dropdownMenu[MaxBaseCraft];
extern int CraftSelectedItem;

#endif


