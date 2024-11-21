#ifndef INPUT_H
#define INPUT_H
#include "raylib.h"
#include "stat.h"
#include <string.h>
#include "texture.h"
#include <stdio.h>

extern int MinPlaceableID ;
extern bool inMouvement;
void InitInventoryKeyBiding();
void mouseDefault();
void leftClic();
void rightClic();
void InteractForeuse();

#define MAX_FOREUSE 100

typedef struct Foreuse {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Foreuse;

extern Foreuse ListeForeuse[MAX_FOREUSE];
extern int numForeuses;  // Compteur de foreuses
void Update_Foreuse();

#define MAX_CONVEYOR 100

typedef struct Conveyor {
    int i;
    int j;
    int dir[2];
    bool placed;
    Texture2D texture;
} Conveyor;

extern Conveyor ListeConveyor[MAX_CONVEYOR];
Texture2D textureToMove;
void ActionWithName(char ObjectName[20],int i,int j);
void Convey(Conveyor conv);
void Update_Conv();
int AddInInvent(int q,Texture2D texture);

#endif