#ifndef CRAFT_H
#define CRAFT_H
#include "cell.h"
#include "inventory.h"

#define MaxBaseCraft 5

void DrawCraftPage();
void CraftButton();
void DrawCraftMenu();
void DrawBaseCraft();
void InitBaseCraft();
void DrawCraftItem();

extern bool DropDownMenu;
typedef enum CraftMenu { NONE,BASE } CraftMenu;
CraftMenu CurrentMenu;


typedef struct Craft {
    Texture2D texture;
    int quantity;
} Craft;

extern Craft BaseCraftInvent[MaxBaseCraft];

Rectangle BaseCraftButton,ConfirmCraftButton1,ConfirmCraftButton5, dropdownMenu[MaxBaseCraft];
extern int CraftSelectedItem;
#endif
