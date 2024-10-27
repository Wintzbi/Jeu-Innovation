#include "craft.h"
#include <stdio.h>

int CraftSelectedItem=-1;
int AvaQuantity=0;
bool IsCraftable=true;
CraftMenu CurrentMenu=NONE;
Craft BaseCraftInvent[MaxBaseCraft];


void CraftButton() {
    BaseCraftButton = (Rectangle) { GetScreenWidth()/2 - 350, 100, 100, 50 };
}

void DrawCraftPage() {
    DrawRectangle(100, 10, GetScreenWidth() - 200, 60, DARKGRAY);
    DrawText("Craft", 330, 11, 60, WHITE);
    DrawCraftMenu();
}

void DrawCraftMenu() {
    CraftButton();
    // Dessiner le bouton principal du menu déroulant
    if (CheckCollisionPointRec(GetMousePosition(), BaseCraftButton)) {
        DrawRectangleRec(BaseCraftButton, LIGHTGRAY); // Couleur si survolé
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentMenu==NONE) {
            CurrentMenu=BASE;  // Ouvrir/fermer le menu
        }
        else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CurrentMenu =NONE;
    } else {
        DrawRectangleRec(BaseCraftButton, GRAY);
    }

    DrawText("Base", BaseCraftButton.x + 10, BaseCraftButton.y + 10, 20, WHITE);

    // Si le menu déroulant est ouvert
    if (CurrentMenu==BASE) {
        DrawBaseCraft();
    }
    if (CurrentMenu!=NONE) {
      CraftSelectedItem=-1;
    }
    // Afficher l'option sélectionnée
    if (CraftSelectedItem >= 0) {
        DrawCraftItem();
}
}
void DrawCraftItem(){
    IsCraftable=true;
    DrawText(TextFormat("%s, %d",BaseCraftInvent[CraftSelectedItem].name,CraftSelectedItem), 400, 180, 30, DARKGRAY);
    Vector2 pos = { 400, 200 };
    DrawTextureEx(BaseCraftInvent[CraftSelectedItem].texture, pos,  0.0f,  8.0f, WHITE);

    for (int i =0;i<BaseCraftInvent[CraftSelectedItem].ComponentCount;i++)
    {
        AvaQuantity=CheckQuantity(BaseCraftInvent[CraftSelectedItem].components[i]);

        if( AvaQuantity>0 ){
            DrawRectangle(screenWidth-210, 200+70*i, 170, 60, DARKGRAY);
        }
        else 
        {
            DrawRectangle(screenWidth-210, 200+70*i, 170, 60, LIGHTGRAY);
            IsCraftable=false;
        }
        DrawText(TextFormat("%d %s",BaseCraftInvent[CraftSelectedItem].components[i].quantity,BaseCraftInvent[CraftSelectedItem].components[i].name), screenWidth-200, 210+70*i, 15, WHITE);
        Vector2 pos = { screenWidth-100, 200+70*i};
        DrawTextureEx(BaseCraftInvent[CraftSelectedItem].components[i].texture, pos,  0.0f,  3.0f, WHITE);

    }

    ConfirmCraftButton1 = (Rectangle) { 300, 400, 120, 50 };
    ConfirmCraftButton5 = (Rectangle) { 450, 400, 120, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), ConfirmCraftButton1)) {
        DrawRectangleRec(ConfirmCraftButton1, LIGHTGRAY); // Couleur si survolé
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsCraftable) {
            CraftItem(1,BaseCraftInvent[CraftSelectedItem]);
            DeletComp(BaseCraftInvent[CraftSelectedItem],1);
        }
    } else {
        DrawRectangleRec(ConfirmCraftButton1, GRAY);
    }
    DrawText(TextFormat("Fabriquer x1"), ConfirmCraftButton1.x + 10, ConfirmCraftButton1.y + 10, 15, DARKGRAY);


    if (CheckCollisionPointRec(GetMousePosition(), ConfirmCraftButton5)) {
            DrawRectangleRec(ConfirmCraftButton5, LIGHTGRAY); // Couleur si survolé
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsCraftable ) {
                CraftItem(5,BaseCraftInvent[CraftSelectedItem]);
                DeletComp(BaseCraftInvent[CraftSelectedItem],5);
            }
        } else {
            DrawRectangleRec(ConfirmCraftButton5, GRAY);
        }
    DrawText(TextFormat("Fabriquer x5"), ConfirmCraftButton5.x + 10, ConfirmCraftButton5.y + 10, 15, DARKGRAY);

}


void InitBaseCraft() {
    BaseCraftInvent[0] = (Craft) {"Lingot Cuivre",copperTexture, 2,2,{{ "Cuivre",copperTexture, 2 },{ "Charbon",coalTexture, 2 }}};
    BaseCraftInvent[1] = (Craft) {"Lingot Fer",ironTexture, 2,2,{{ "Fer",ironTexture, 2 },{ "Charbon",coalTexture, 2 }}};
    // Initialiser les rectangles des options de menu
    for (int i = 0; i < MaxBaseCraft; i++) {
        dropdownMenu[i] = (Rectangle) { screenWidth-(screenWidth-100), 150 + (i + 1) * 50, 200, 40 };
    }
}

void DrawBaseCraft() {
    for (int i = 0; i < MaxBaseCraft; i++) {
            if (CheckCollisionPointRec(GetMousePosition(), dropdownMenu[i])) {
                DrawRectangleRec(dropdownMenu[i], LIGHTGRAY);  // Survolé
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    CraftSelectedItem = i;  // Option sélectionnée
                    printf("CraftSelectedItem = %d\n",CraftSelectedItem);
                    CurrentMenu=NONE;  // Fermer le menu après sélection
                }
            } else {
                DrawRectangleRec(dropdownMenu[i], GRAY);
            }

            DrawText(TextFormat("%s",BaseCraftInvent[i].name), dropdownMenu[i].x + 10, dropdownMenu[i].y + 10, 20, DARKGRAY);
        }

    
}

int CheckQuantity(Item comp){
    int res=0;
    for (int i=0;i<INVENTORY_SIZE;i++){
        //printf("nom comp : %s, inventaire : %s\n",comp.name,inventory[i].name);
        if (strcmp(comp.name, inventory[i].name) ==0 ){
            //printf("Quantité %d et %d",inventory[i].quantity,comp.quantity);
            res+=inventory[i].quantity;
        }
    }
    if (res<comp.quantity) return -1;
    else if (res>=comp.quantity) return 1;
    else if (res>=5*comp.quantity) return 5;
    return -2;
}

int CraftItem(int q,Craft obj){
    for (int i=0;i<INVENTORY_SIZE;i++){
        if (strcmp(obj.name, inventory[i].name) ==0  )
        {

            inventory[i].quantity += q;
            return 0;
        }
    }
    for (int i=0;i<INVENTORY_SIZE;i++){
        if (inventory[i].quantity==0)
        {
            strncpy(inventory[i].name, obj.name, sizeof(inventory[i].name) );
            //inventory[i].name[sizeof(inventory[i].name) - 1] = '\0'; // S'assurer que c'est terminé par '\0'
            inventory[i].quantity = q;
            inventory[i].texture = obj.texture;
            return 0;
        }
    }
    IsCraftable=true;
    return -1;
}
int DeletComp(Craft obj,int q){
    for (int j =0;j<obj.ComponentCount;j++){
        for (int i=0;i<INVENTORY_SIZE;i++){
            if (inventory[i].quantity !=0){
                if ((strcmp(obj.components[j].name, inventory[i].name) ==0) || (obj.components[j].texture.id == inventory[i].texture.id)){
                    inventory[i].quantity -= q * obj.components[j].quantity;
                    break;
                }
            }
        }
    }
    return 0;
}