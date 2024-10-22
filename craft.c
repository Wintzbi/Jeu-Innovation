#include "craft.h"

void DrawCraftPage(){
    DrawRectangle(100, 10, screenWidth-200, 60, DARKGRAY);
    DrawText(TextFormat("Craft"), 330, 11, 60, WHITE); // Display quantity
    DrawCraftMenu();
}

Rectangle BaseCraftButton;

void CraftButton() {
    BaseCraftButton = (Rectangle) {screenWidth/2-350, 100, 100, 50 };
    
}

void DrawCraftMenu(){
        CraftButton();

    // Dessiner les bouttons
    if (CheckCollisionPointRec(GetMousePosition(), BaseCraftButton)) {
        DrawRectangleRec(BaseCraftButton, LIGHTGRAY); // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        }
    }
        else DrawRectangleRec(BaseCraftButton, GRAY);
    DrawText(TextFormat("Base"), BaseCraftButton.x+10, 100, 20, WHITE);
}
