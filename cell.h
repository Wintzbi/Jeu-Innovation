#ifndef CELL_H
#define CELL_H
#include "raylib.h"
#include "texture.h"
#include <stdio.h>   // Pour printf
#include <stdlib.h>  // Pour rand et srand
#include <time.h>    // Pour time

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

typedef struct Generator {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Generator;

extern Cell grid[ROW][COL];
extern Generator MineraiCuivreGenerator;
void CellDraw(Cell cell);
bool IndexIsValid(int i, int j);
void InitGrid();
void GridDraw();
void InitMineraiGenerator();
void MineraiGenerator();

#endif