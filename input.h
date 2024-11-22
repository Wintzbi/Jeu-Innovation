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
    int q;
    Texture2D texture;
} Foreuse;

extern Foreuse ListeForeuse[MAX_FOREUSE];
extern int numForeuses;  // Compteur de foreuses
void Update_Foreuse();
extern Foreuse* NearForeuse;  // Pointeur vers Foreuse

#define MAX_CONVEYOR 100

typedef struct Conveyor {
    int i;
    int j;
    int dir[2];
    bool placed;
    Texture2D texture;
    bool inMouvement;    // Indique si un objet est en mouvement sur ce convoyeur
    Texture2D textureToMove;
} Conveyor;
extern Conveyor ListeConveyor[MAX_CONVEYOR];
void ActionWithName(char ObjectName[20],int i,int j,int option);
void Convey(Conveyor *conv);
void UpdateDir();
void Update_Conv();
extern int conveyor_dir;

int AddInInvent(int q,Texture2D texture);
extern int directions[4][2] ;

#endif