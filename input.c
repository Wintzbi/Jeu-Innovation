#include "cell.h"

void mouseDefault() {
    int posX = (int)(GetMousePosition().x / cellSize);
    int posY = (int)(GetMousePosition().y / cellSize);
    DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(RED, 0.3f));
}