#include "cell.h"
#include "inventory.h"

const int screenWidth = 1920;
const int screenHeight = 1200;
const int cellSize = screenWidth / COL;
int offsetX = 0;
int offsetY = 0;

// Fonction pour dessiner une cellule
void CellDraw(Cell cell) {
    if (cell.placed) {
        int offsetX = (cellSize - cell.texture.width) / 2;
        int offsetY = (cellSize - cell.texture.height) / 2;
        DrawTexture(cell.texture, cell.i * cellSize + offsetX, cell.j * cellSize + offsetY, WHITE);
    }
    DrawRectangleLines(cell.i * cellSize, cell.j * cellSize, cellSize, cellSize, LIGHTGRAY);
}

// Fonction pour vérifier si les indices de la grille sont valides
bool IndexIsValid(int i, int j) {
    return (i >= 0 && i < COL && j >= 0 && j < ROW);
}

void InitGrid() {
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            grid[i][j] = (Cell) {
                .i = i,
                .j = j,
                .placed = false,
                .texture = copperTexture  // Par défaut, on peut utiliser n'importe quelle texture
            };
        }
    }
}

void GridDraw() {
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            CellDraw(grid[i][j]);
        }
    }
}