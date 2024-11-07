#ifndef CELL_H
#define CELL_H
#include "raylib.h"
#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COL 200
#define ROW 200
#define MAX_GENERATORS 8
#define MAX_FOREUSES 8

typedef struct Cell {
    int i;
    int j;
    bool placed;
    bool pickable;
    Texture2D texture;
} Cell;

typedef struct Generator {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Generator;

typedef struct Foreuse {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Foreuse;

extern Generator generators[MAX_GENERATORS];
extern Cell grid[ROW][COL];

extern const int screenWidth;
extern const int screenHeight;
extern const int cellSize;

void CellDraw(Cell cell);
bool IndexIsValid(int i, int j);
void InitGrid();
void GridDraw();
void InitMineraiGenerator();
void MineraiGenerator();

#endif
