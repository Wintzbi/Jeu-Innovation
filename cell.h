#ifndef CELL_H
#define CELL_H
#include "raylib.h"
#include "texture.h"
#include <stdio.h>   // Pour printf
#include <stdlib.h>  // Pour rand et srand
#include <time.h>    // Pour time

extern int screenWidth;
extern int screenHeight;
extern int cellSize;

#define COL 200
#define ROW 200

#define MAX_GENERATORS 8

extern int offsetX ;
extern int offsetY;

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

 // Définir le nombre maximal de générateurs

extern Generator generators[MAX_GENERATORS]; // Tableau de générateurs

extern Cell grid[ROW][COL];
extern Generator MineraiCuivreGenerator;
void InitScreenSize();
void CellDraw(Cell cell);
bool IndexIsValid(int i, int j);
void InitGrid();
void GridDraw();
void InitMineraiGenerator();
void MineraiGenerator();

#endif