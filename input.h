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
#define MAX_FURNACE 100

typedef struct Foreuse {
    int i;
    int j;
    bool placed;
    int q;
    Texture2D texture;
} Foreuse;

typedef struct Furnace {
    int i;
    int j;
    bool placed;
    int energy_q;
    int energy_id;
    int material_q;
    int matrial_id;
    int final_q;
    Texture2D texture;
} Furnace;

extern Foreuse ListeForeuse[MAX_FOREUSE];
extern int numForeuses;  // Compteur de foreuses
void Update_Foreuse();
extern Foreuse* NearForeuse;  // Pointeur vers Foreuse

extern Furnace ListeFurnace[MAX_FURNACE];
extern int numFurnaces;  // Compteur de foreuses
void Update_Furnace();

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
void ActionWithName(char ObjectName[20],int i,int j);
void Convey(Conveyor *conv);
void Update_Conv();
int AddInInvent(int q,Texture2D texture);

#endif