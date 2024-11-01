#ifndef CRAFT_H
#define CRAFT_H
#include "cell.h"
#include "inventory.h"

#define MaxBaseCraft 5

typedef struct Craft {
    char name[20];
    Texture2D texture;
    int quantity;
    int ComponentCount;
    Item components[5]; // Composants nécessaires
} Craft;

typedef enum CraftMenu { NONE,BASE,STRUCT } CraftMenu;
CraftMenu CurrentMenu;
CraftMenu OldMenu;

void DrawCraftPage();
void CraftButton();
void DrawCraftMenu();
void DrawBaseCraft();
void InitBaseCraft();
void DrawCraftItem(CraftMenu OldMenu);
void DrawStructCraft();
int CraftItem(int q,Craft obj);
int CheckQuantity(Item comp);
int DeletComp(Craft obj,int q);
extern int AvaQuantity;

extern bool DropDownMenu;
extern bool IsCraftable;




extern Craft BaseCraftInvent[MaxBaseCraft];
extern Craft StructCraftInvent[MaxBaseCraft];

Rectangle BaseCraftButton,StructCraftButton,ConfirmCraftButton1,ConfirmCraftButton5;
Rectangle BaseDropdownMenu[MaxBaseCraft] ,StructDropdownMenu[MaxBaseCraft];
extern int CraftSelectedItem;
#endif
