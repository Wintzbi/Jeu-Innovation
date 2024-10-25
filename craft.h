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

void DrawCraftPage();
void CraftButton();
void DrawCraftMenu();
void DrawBaseCraft();
void InitBaseCraft();
void DrawCraftItem();
int CraftItem(int q,Craft obj);
int CheckQuantity(Item comp);
int DeletComp(Craft obj);

extern bool DropDownMenu;
extern bool IsCraftable;
typedef enum CraftMenu { NONE,BASE } CraftMenu;
CraftMenu CurrentMenu;



extern Craft BaseCraftInvent[MaxBaseCraft];

Rectangle BaseCraftButton,ConfirmCraftButton1,ConfirmCraftButton5, dropdownMenu[MaxBaseCraft];
extern int CraftSelectedItem;
#endif
