#ifndef INPUT_H
#define INPUT_H

// Inclusions
#include "raylib.h"
#include "stat.h"
#include "craft.h"
#include <string.h>
#include "texture.h"
#include <stdio.h>

// Déclarations externes générales
extern int MinPlaceableID;
extern bool inMouvement;

// Constantes
#define MAX_FOREUSE 100
#define MAX_FURNACE 100
#define MAX_HYDRAULIC 100
#define MAX_ETTIREUSE 100
#define MAX_STEAM 100
#define MAX_OIL 100
#define MAX_CONVEYOR 400
#define MAX_BATTERY 100

// Directions pour les mouvements
extern int directions[4][2];

// Structures

// Foreuse
typedef struct Foreuse {
    int i;
    int j;
    bool placed;
    int q;
    Texture2D texture;
} Foreuse;
extern Foreuse ListeForeuse[MAX_FOREUSE];
extern int numForeuses;
extern Foreuse* NearForeuse;
void Update_Foreuse();

// Furnace
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
extern Furnace ListeFurnace[MAX_FURNACE];
extern int numFurnaces;
void Update_Furnace();

// Hydraulic
typedef struct Hydraulic {
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
} Hydraulic;
extern Hydraulic ListeHydraulic[MAX_HYDRAULIC];
extern int numHydraulics;
void Update_Hydraulic();

// Ettireuse
typedef struct Ettireuse {
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
} Ettireuse;
extern Ettireuse ListeEttireuse[MAX_ETTIREUSE];
extern int numEttireuses;
void Update_Ettireuse();

// Steam
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
extern Steam ListeSteam[MAX_STEAM];
extern int numSteams;
void Update_Steam();
int FindNearestSteam(int x, int y);

// Oil
typedef struct Oil {
    int i;
    int j;
    bool placed;
    int energy_q;
    int energy_id;
    int material_q;
    int material_id;
    int final_q;
    Texture2D texture;
} Oil;
extern Oil ListeOil[MAX_OIL];
extern int numOils;
void Update_Oil();

// Conveyor
typedef struct Conveyor {
    int i;
    int j;
    int dir[2];
    bool placed;
    bool power;
    Texture2D texture;
    bool inMouvement;
    Texture2D textureToMove;
} Conveyor;
extern Conveyor ListeConveyor[MAX_CONVEYOR];
void Convey(Conveyor* conv);
void UpdateDir();
void Update_Conv();
extern int conveyor_dir;
void RemoveConveyor(int posX, int posY);

// Battery
typedef struct Battery {
    int i;
    int j;
    int q;
    bool placed;
    Texture2D texture;
} Battery;
extern Battery ListeBattery[MAX_BATTERY];
void UpdateBattery();
int FindNearestBattery(int x, int y);
void RemoveBattery(int posX, int posY);

// Fonctions utilitaires
void InitInventoryKeyBiding();
void mouseDefault();
void leftClic();
void rightClic();
void ActionWithName(char ObjectName[20], int i, int j, int option);
int AddInInvent(int q, Texture2D texture);
const char* FindName(Texture2D textureRef);
int IsEnergieNear(int x, int y, int range);

#endif
