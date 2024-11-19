#ifndef INPUT_H
#define INPUT_H
#include "raylib.h"
#include "stat.h"
extern int MinPlaceableID ;
void InitInventoryKeyBiding();
void mouseDefault();
void leftClic();
void rightClic();
void InteractForeuse();

typedef struct Foreuse {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Foreuse;

#endif