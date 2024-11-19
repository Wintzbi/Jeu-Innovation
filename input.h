#ifndef INPUT_H
#define INPUT_H
#include "raylib.h"
#include "stat.h"

#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
extern int MinPlaceableID ;
void InitInventoryKeyBiding();
void mouseDefault();
void leftClic();
void rightClic();

#define MAX_CONVEYOR 100

typedef struct Conveyor {
    int i;
    int j;
    int dir[2];
    bool placed;
    Texture2D texture;
} Conveyor;

extern Conveyor ListeConveyor[MAX_CONVEYOR];

void Convey(Conveyor conv);
void Update_Conv();

#endif