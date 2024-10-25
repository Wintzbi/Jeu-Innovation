#include "craft.h"

int CraftSelectedItem=-1;

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
    DrawText(TextFormat("Selected: Option %d", CraftSelectedItem + 1), 400, 180, 20, DARKGREEN);
    Vector2 pos = { 400, 200 };
    DrawTextureEx(BaseCraftInvent[CraftSelectedItem].texture, pos,  0.0f,  8.0f, WHITE);

    ConfirmCraftButton1 = (Rectangle) { 300, 400, 120, 50 };
    ConfirmCraftButton5 = (Rectangle) { 450, 400, 120, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), ConfirmCraftButton1)) {
        DrawRectangleRec(ConfirmCraftButton1, LIGHTGRAY); // Couleur si survolé
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentMenu==NONE) {
        }
        else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ;
    } else {
        DrawRectangleRec(ConfirmCraftButton1, GRAY);
    }
    DrawText(TextFormat("Fabriquer x1"), ConfirmCraftButton1.x + 10, ConfirmCraftButton1.y + 10, 15, DARKGRAY);


    if (CheckCollisionPointRec(GetMousePosition(), ConfirmCraftButton5)) {
            DrawRectangleRec(ConfirmCraftButton5, LIGHTGRAY); // Couleur si survolé
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentMenu==NONE) {
            }
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ;
        } else {
            DrawRectangleRec(ConfirmCraftButton5, GRAY);
        }
    DrawText(TextFormat("Fabriquer x5"), ConfirmCraftButton5.x + 10, ConfirmCraftButton5.y + 10, 15, DARKGRAY);

}


void InitBaseCraft() {
    BaseCraftInvent[0] = (Craft) { "Cuivre",copperTexture, 2 };

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
                    CurrentMenu=NONE;  // Fermer le menu après sélection
                }
            } else {
                DrawRectangleRec(dropdownMenu[i], GRAY);
            }

            DrawText(TextFormat("%s",BaseCraftInvent[i].name), dropdownMenu[i].x + 10, dropdownMenu[i].y + 10, 20, DARKGRAY);
        }

    
}
