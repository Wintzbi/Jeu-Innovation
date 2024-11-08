#ifndef CELL_H
#define CELL_H
#include "raylib.h"
#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COL 200
#define ROW 200
#define MAX_GENERATORS 20
#define MAX_FOREUSES 8

typedef struct Cell {
    int i;
    int j;
    bool placed;
    bool pickable;
    Texture2D texture; //texture sol
    Texture2D up_texture;//texture suplémentaire
} Cell;

typedef struct Generator {
    int i;
    int j;
    int max_number;
    int density;
    bool placed;
    Texture2D texture;
    Texture2D up_texture;
} Generator;

typedef struct Foreuse {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Foreuse;

typedef struct Filon {
    int max_number;
    int density;
    Texture2D texture;
    Texture2D up_texture;
} Filon;

extern Generator generators[MAX_GENERATORS];
extern Foreuse ListeForeuse[MAX_FOREUSES];
extern Filon ListeFilon[2];  
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
