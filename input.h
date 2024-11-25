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

#define MAX_FOREUSE 100
#define MAX_FURNACE 100
#define MAX_STEAM 100

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
    int material_id;
    int final_q;
    int final_id;
    Texture2D texture;
} Furnace;

typedef struct Steam {
    int i;
    int j;
    bool placed;
    int energy_q;
    int energy_id;
    int material_q;
    int material_id;
    int final_q;
    Texture2D texture;
} Steam;

extern Foreuse ListeForeuse[MAX_FOREUSE];
extern int numForeuses;  // Compteur de foreuses
void Update_Foreuse();
extern Foreuse* NearForeuse;  // Pointeur vers Foreuse

extern Furnace ListeFurnace[MAX_FURNACE];
extern int numFurnaces;  // Compteur de foreuses
void Update_Furnace();

extern Steam ListeSteam[MAX_STEAM];
extern int numSteams;  // Compteur de foreuses
void Update_Steam();
int FindNearestCentral(int x, int y);

#define MAX_CONVEYOR 400


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
void RemoveConveyor(int posX, int posY);
int IsEnergieNear(int x,int y,int range);

#define MAX_BATTERY 100

typedef struct Battery {
    int i;
    int j;
    int q;
    bool placed;
    Texture2D texture;
}Battery;

extern Battery ListeBattery[MAX_BATTERY];
void UpdateBattery();
int FindNearestBattery(int x, int y);
void RemoveBattery(int posX, int posY);
#endif