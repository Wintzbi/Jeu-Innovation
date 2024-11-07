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

typedef struct Foreuse {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Foreuse;

#define MAX_GENERATORS 8 // Définir le nombre maximal de générateurs
#define MAX_FOREUSES 8
extern Generator generators[MAX_GENERATORS]; // Tableau de générateurs
extern Foreuse ListeForeuse[MAX_FOREUSES];

extern Cell grid[ROW][COL];
extern Generator MineraiCuivreGenerator;
void CellDraw(Cell cell);
bool IndexIsValid(int i, int j);
void InitGrid();
void GridDraw();
void InitMineraiGenerator();
void MineraiGenerator();

#endif