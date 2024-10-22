#include "craft.h"

void DrawCraftPage(){
    DrawRectangle(100, 10, screenWidth-200, 60, DARKGRAY);
    DrawText(TextFormat("Craft"), 330, 11, 60, WHITE); // Display quantity

}