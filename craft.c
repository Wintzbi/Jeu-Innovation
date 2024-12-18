#include "craft.h"
#include <stdio.h>

int CraftSelectedItem=-1;
int AvaQuantity=0;
bool IsCraftable=true;
CraftMenu CurrentMenu=NONE;
CraftMenu OldMenu=NONE;
Craft *currentInventory = NULL;
Rectangle (*currentDropDownMenu)[MaxBaseCraft] = NULL;
Craft BaseCraftInvent[MaxBaseCraft];
Craft StructCraftInvent[MaxBaseCraft];
Craft MecaCraftInvent[MaxBaseCraft];
Craft ProdCraftInvent[MaxBaseCraft];

Rectangle BaseCraftButton,StructCraftButton,MecaCraftButton,ProdCraftButton,ConfirmCraftButton1,ConfirmCraftButton5;
Rectangle BaseDropdownMenu[MaxBaseCraft] ,StructDropdownMenu[MaxBaseCraft],MecaDropdownMenu[MaxBaseCraft],ProdDropdownMenu[MaxBaseCraft];



void InitBaseCraft() {
    BaseCraftInvent[0] = (Craft) {"Lingot Cuivre",copperLingotTexture, 2,2,{{ "Cuivre",copperTexture, 2 },{ "Charbon",coalTexture, 2 }}};
    BaseCraftInvent[1] = (Craft) {"Lingot Fer",ironLingotTexture, 2,2,{{ "Fer",ironTexture, 2 },{ "Charbon",coalTexture, 2 }}};
    BaseCraftInvent[2] = (Craft) {"Lingot Acier",steelTexture, 2,2,{{ "Lingot Fer",ironLingotTexture, 1 },{ "Charbon",coalTexture, 2 }}};
    

    StructCraftInvent[0] = (Craft) {"Barre Fer",ironRodTexture, 2,1,{{ "Lingot Fer",ironLingotTexture, 2 }}};
    StructCraftInvent[1] = (Craft) {"Plaque Fer",ironPlateTexture, 2,1,{{ "Lingot Fer",ironLingotTexture, 2 }}};
    StructCraftInvent[2] = (Craft) {"Barre cuivre",copperRodTexture, 2,1,{{ "Lingot Cuivre",copperLingotTexture, 2 }}};
    StructCraftInvent[3] = (Craft) {"Plaque cuivre",copperPlateTexture, 2,1,{{ "Lingot Cuivre",copperLingotTexture, 2 }}};
    
    
    MecaCraftInvent[0] = (Craft) {"Engrenage",gearTexture, 2,1,{{ "Plaque Fer",ironPlateTexture, 1 }}};
    MecaCraftInvent[1] = (Craft) {"Tuyau",pipeTexture, 2,2,{{ "Barre cuivre",copperRodTexture, 1 },{ "Plaque cuivre",copperPlateTexture, 1 }}};
    MecaCraftInvent[2] = (Craft) {"cable",pipeTexture, 2,1,{{ "Barre cuivre",copperRodTexture, 1 }}};


    ProdCraftInvent[0] = (Craft) {"Tapis",conveyorTexture, 2,3,{{ "Plaque Fer",ironPlateTexture, 1 },{ "Barre cuivre",copperRodTexture, 1 },{ "Engrenage",gearTexture, 1 }}};
    ProdCraftInvent[1] = (Craft) {"Foreuse",drillTexture, 2,3,{{ "Plaque Fer",ironPlateTexture, 1 },{ "Barre cuivre",copperRodTexture, 1 },{ "Engrenage",gearTexture, 1 }}};
    ProdCraftInvent[2] = (Craft) {"Furnace",furnaceTexture, 2,3,{{ "Plaque Fer",ironPlateTexture, 1 },{ "Barre cuivre",copperRodTexture, 1 },{ "Engrenage",gearTexture, 1 }}};
    ProdCraftInvent[3] = (Craft) {"Centrale Vapeur",steamcentralTexture, 2,3,{{ "Plaque Fer",ironPlateTexture, 1 },{ "Barre cuivre",copperRodTexture, 1 },{ "Engrenage",gearTexture, 1 }}};
    ProdCraftInvent[3] = (Craft) {"Coffre",chestTexture, 2,3,{{"Plaque Fer",ironPlateTexture, 1 },{ "Barre cuivre",copperRodTexture, 1 },{ "Engrenage",gearTexture, 1 }}};
    ProdCraftInvent[4] = (Craft) {"Batterie",batteryTexture, 2,3,{{ "Plaque Fer",ironPlateTexture, 1 },{ "Barre cuivre",copperRodTexture, 1 },{ "Engrenage",gearTexture, 1 }}};


    // Initialiser les rectangles des options de menu
    for (int i = 0; i < MaxBaseCraft; i++) {
        BaseDropdownMenu[i] = (Rectangle) { screenWidth-(screenWidth-100), 150 + (i + 1) * 50, 200, 40 };
        StructDropdownMenu[i] = (Rectangle) { screenWidth-(screenWidth-100), 150 + (i + 1) * 50, 200, 40 };
        MecaDropdownMenu[i] = (Rectangle) { screenWidth-(screenWidth-100), 150 + (i + 1) * 50, 200, 40 };
        ProdDropdownMenu[i] = (Rectangle) { screenWidth-(screenWidth-100), 150 + (i + 1) * 50, 200, 40 };
    }
    
}

Rectangle BaseCraftButton, dropdownMenu[MaxBaseCraft];

void CraftButton() {
    BaseCraftButton = (Rectangle) { GetScreenWidth()/2 - 350, 100, 100, 50 };
    StructCraftButton= (Rectangle) { GetScreenWidth()/2 - 240, 100, 100, 50 };
    MecaCraftButton= (Rectangle) { GetScreenWidth()/2 - 130, 100, 100, 50 };
    ProdCraftButton= (Rectangle) { GetScreenWidth()/2 - 20, 100, 100, 50 };
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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentMenu!=BASE) {
            CurrentMenu=BASE;  // Ouvrir/fermer le menu
        }
        else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CurrentMenu =NONE;
    } else {
        DrawRectangleRec(BaseCraftButton, GRAY);
    }

    DrawText("Base", BaseCraftButton.x + 10, BaseCraftButton.y + 10, 20, WHITE);
        
    if (CheckCollisionPointRec(GetMousePosition(), StructCraftButton)) {
            DrawRectangleRec(StructCraftButton, LIGHTGRAY); // Couleur si survolé
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentMenu!=STRUCT) {
                CurrentMenu=STRUCT;  // Ouvrir/fermer le menu
            }
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CurrentMenu =NONE;
        } else {
            DrawRectangleRec(StructCraftButton, GRAY);
        }

        DrawText("Struct", StructCraftButton.x + 10, StructCraftButton.y + 10, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), MecaCraftButton)) {
            DrawRectangleRec(MecaCraftButton, LIGHTGRAY); // Couleur si survolé
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentMenu!=MECA) {
                CurrentMenu=MECA;  // Ouvrir/fermer le menu
            }
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CurrentMenu =NONE;
        } else {
            DrawRectangleRec(MecaCraftButton, GRAY);
        }

        DrawText("Meca", MecaCraftButton.x + 10, MecaCraftButton.y + 10, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), ProdCraftButton)) {
            DrawRectangleRec(ProdCraftButton, LIGHTGRAY); // Couleur si survolé
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentMenu!=PROD) {
                CurrentMenu=PROD;  // Ouvrir/fermer le menu
            }
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CurrentMenu =NONE;
        } else {
            DrawRectangleRec(ProdCraftButton, GRAY);
        }

        DrawText("Prod", ProdCraftButton.x + 10, ProdCraftButton.y + 10, 20, WHITE);



    // Si le menu déroulant est ouvert
    if (CurrentMenu==BASE) {
        OldMenu=BASE;
        currentInventory = BaseCraftInvent;
        currentDropDownMenu=&BaseDropdownMenu;
        DrawBaseCraft();
        
    }
   if (CurrentMenu==STRUCT) {
        OldMenu=STRUCT;
        currentInventory = StructCraftInvent;
        currentDropDownMenu = &StructDropdownMenu;
        DrawBaseCraft();
    }
    if (CurrentMenu==MECA) {
        OldMenu=MECA;
        currentInventory = MecaCraftInvent;
        DrawBaseCraft();}

    if (CurrentMenu==PROD) {
        OldMenu=PROD;
        currentInventory = ProdCraftInvent;
        DrawBaseCraft();
    }
    if (CurrentMenu!=NONE) {
      CraftSelectedItem=-1;
    }
    // Afficher l'option sélectionnée
    if (CraftSelectedItem >= 0) {
        IsCraftable=true;
        DrawCraftItem(OldMenu);
}
}

void DrawCraftItem(CraftMenu OldMenu){


    DrawText(TextFormat("%s",currentInventory[CraftSelectedItem].name), 400, 180, 30, DARKGRAY);
    Vector2 pos = { 400, 200 };
    DrawTextureEx(currentInventory[CraftSelectedItem].texture, pos,  0.0f,  4.0f, WHITE);

    for (int i =0;i<currentInventory[CraftSelectedItem].ComponentCount;i++)
    {
        AvaQuantity=CheckQuantity(currentInventory[CraftSelectedItem].components[i]);

        if( AvaQuantity>0 ){
            DrawRectangle(screenWidth-210, 200+70*i, 170, 60, DARKGRAY);
        }
        else 
        {
            DrawRectangle(screenWidth-210, 200+70*i, 170, 60, LIGHTGRAY);
            IsCraftable=false;
        }
        DrawText(TextFormat("%d %s",currentInventory[CraftSelectedItem].components[i].quantity,currentInventory[CraftSelectedItem].components[i].name), screenWidth-200, 210+70*i, 15, WHITE);
        Vector2 pos = { screenWidth-100, 200+70*i};
        DrawTextureEx(currentInventory[CraftSelectedItem].components[i].texture, pos,  0.0f,  2.0f, WHITE);

    }

    ConfirmCraftButton1 = (Rectangle) { 300, 400, 120, 50 };
    ConfirmCraftButton5 = (Rectangle) { 450, 400, 120, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), ConfirmCraftButton1)) {
        DrawRectangleRec(ConfirmCraftButton1, LIGHTGRAY); // Couleur si survolé
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsCraftable) {
            CraftItem(1,currentInventory[CraftSelectedItem]);
            craftedObjects+=1;
            DeletComp(currentInventory[CraftSelectedItem],1);
        }
    } else {
        DrawRectangleRec(ConfirmCraftButton1, GRAY);
    }
    DrawText(TextFormat("Fabriquer x1"), ConfirmCraftButton1.x + 10, ConfirmCraftButton1.y + 10, 15, DARKGRAY);


    if (CheckCollisionPointRec(GetMousePosition(), ConfirmCraftButton5)) {
            DrawRectangleRec(ConfirmCraftButton5, LIGHTGRAY); // Couleur si survolé
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsCraftable ) {
                CraftItem(5,currentInventory[CraftSelectedItem]);
                craftedObjects+=5;
                DeletComp(currentInventory[CraftSelectedItem],5);
            }
        } else {
            DrawRectangleRec(ConfirmCraftButton5, GRAY);
        }
    DrawText(TextFormat("Fabriquer x5"), ConfirmCraftButton5.x + 10, ConfirmCraftButton5.y + 10, 15, DARKGRAY);

}

void DrawBaseCraft() {
    if (currentDropDownMenu != NULL){
        for (int i = 0; i < MaxBaseCraft; i++) {
                if (CheckCollisionPointRec(GetMousePosition(), (*currentDropDownMenu)[i])) {
                    DrawRectangleRec((*currentDropDownMenu)[i], LIGHTGRAY);  // Survolé
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&& currentInventory[i].quantity!=0) {
                        CraftSelectedItem = i;  // Option sélectionnée
                        CurrentMenu=NONE;  // Fermer le menu après sélection
                    }
                } else {
                    DrawRectangleRec((*currentDropDownMenu)[i], GRAY);
                }

                DrawText(TextFormat("%s",currentInventory[i].name), (*currentDropDownMenu)[i].x + 10, (*currentDropDownMenu)[i].y + 10, 20, DARKGRAY);
            }

    }
}



int CheckQuantity(Item comp){
    int res=0;
    for (int i=0;i<INVENTORY_SIZE;i++){
        //printf("nom comp : %s, inventaire : %s\n",comp.name,inventory[i].name);
        if (comp.texture.id == inventory[i].texture.id ){
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