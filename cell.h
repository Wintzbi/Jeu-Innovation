
#include "raylib.h"

extern const int screenWidth;
extern const int screenHeight;
extern const int cellSize;
#define COL 50
#define ROW 50



extern int offsetX ;
extern int offsetY;

typedef struct Cell {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Cell;

Cell grid[ROW][COL];
void CellDraw(Cell cell);
bool IndexIsValid(int i, int j);