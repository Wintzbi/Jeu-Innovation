#ifndef CELL_H
#define CELL_H
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "stat.h"

#define COL 200
#define ROW 200
#define MAX_GENERATORS 200
#define NB_FILON 7

extern float rotation;

typedef struct Cell {
    int i;
    int j;
    bool placed;
    bool pickable;
    bool moveable;
    bool isSolid;
    int dir[2];
    Texture2D texture; //texture sol
    Texture2D up_texture;//texture suplémentaire
   Texture2D move_texture;
} Cell;

typedef struct Generator {
    int i;
    int j;
    int max_number;
    int density;
    bool placed;
    bool isSolid;
    Texture2D texture;
    Texture2D up_texture;
} Generator;

typedef struct Filon {
    int max_number;
    int density;
    bool isSolid;
    Texture2D texture;
    Texture2D up_texture;
} Filon;

extern Generator generators[MAX_GENERATORS];
extern Filon ListeFilon[NB_FILON];  
extern Cell grid[ROW][COL];

extern int screenWidth;
extern int screenHeight;
extern int cellSize;

void CellDraw(Cell cell);
void update_size();
bool IndexIsValid(int i, int j);
void InitGrid();
void GridDraw();
void InitMineraiGenerator();
void MineraiGenerator();
int GetDirectionAngle(int direction[2]);
void DrawMap();
int Clamp(int value, int min, int max);
int DayAndNight();


#endif
