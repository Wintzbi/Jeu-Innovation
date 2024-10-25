#include "craft.h"
#include "raylib.h"

int CraftSelectedItem=-1;
bool DropDownMenu = false;

Rectangle BaseCraftButton, dropdownMenu[MaxBaseCraft];

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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DropDownMenu = !DropDownMenu;  // Ouvrir/fermer le menu
        }
    } else {
        DrawRectangleRec(BaseCraftButton, GRAY);
    }

    DrawText("Base", BaseCraftButton.x + 10, BaseCraftButton.y + 10, 20, WHITE);

    // Si le menu déroulant est ouvert
    if (DropDownMenu) {
        for (int i = 0; i < MaxBaseCraft; i++) {
            if (CheckCollisionPointRec(GetMousePosition(), dropdownMenu[i])) {
                DrawRectangleRec(dropdownMenu[i], LIGHTGRAY);  // Survolé
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    CraftSelectedItem = i;  // Option sélectionnée
                    DropDownMenu = false;  // Fermer le menu après sélection
                }
            } else {
                DrawRectangleRec(dropdownMenu[i], GRAY);
            }

            DrawText(TextFormat("Option %d", i + 1), dropdownMenu[i].x + 10, dropdownMenu[i].y + 10, 20, DARKGRAY);
        }
    }

    // Afficher l'option sélectionnée
    if (CraftSelectedItem >= 0) {
        DrawText(TextFormat("Selected: Option %d", CraftSelectedItem + 1), 100, 400, 20, DARKGREEN);
    }
}

void InitBaseCraft() {
    // Remplir BaseCraft avec des textures fictives (à remplacer par vos vraies textures)
    for (int i = 0; i < MaxBaseCraft; i++) {
        //BaseCraft[i] = (Craft) { LoadTexture("path_to_texture.png"), 1 };
    }

    // Initialiser les rectangles des options de menu
    for (int i = 0; i < MaxBaseCraft; i++) {
        dropdownMenu[i] = (Rectangle) { BaseCraftButton.x, BaseCraftButton.y + (i + 1) * 50, 200, 40 };
    }
}

void DrawBaseCraft() {
    // Dessiner les options si le menu est ouvert
    if (DropDownMenu) {
        for (int i = 0; i < MaxBaseCraft; i++) {
            DrawRectangleRec(dropdownMenu[i], LIGHTGRAY);
            DrawText(TextFormat("Item %d", i + 1), dropdownMenu[i].x + 10, dropdownMenu[i].y + 10, 20, DARKGRAY);
        }
    }

    
}
