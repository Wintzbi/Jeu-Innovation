#ifndef INPUT_H
#define INPUT_H

#include "raylib.h"
#include "stat.h"
#include "craft.h"
#include "texture.h"
#include "machine.h"
#include <string.h>
#include <stdio.h>

// ─── Constantes ──────────────────────────────────────────────────────────────
extern int MinPlaceableID;
extern float rotation;
extern int conveyor_dir;

#define MAX_FOREUSE   100
#define MAX_FURNACE   100
#define MAX_HYDRAULIC 100
#define MAX_ETTIREUSE 100
#define MAX_STEAM     100
#define MAX_OIL       100
#define MAX_CONVEYOR  400
#define MAX_BATTERY   100

extern int directions[4][2];
extern bool infoMode;

// ─── Foreuse ──────────────────────────────────────────────────────────────────
typedef struct Foreuse {
    int i, j;
    bool placed;
    int q;
    Texture2D texture;
} Foreuse;
extern Foreuse ListeForeuse[MAX_FOREUSE];
extern int numForeuses;
extern float lastForeuseTime;

// ─── Machines processeurs ─────────────────────────────────────────────────────
extern Machine ListeFurnace[MAX_FURNACE];
extern int numFurnaces;
extern float lastFurnaceTime;

extern Machine ListeHydraulic[MAX_HYDRAULIC];
extern int numHydraulics;
extern float lastHydraulicTime;

extern Machine ListeEttireuse[MAX_ETTIREUSE];
extern int numEttireuses;
extern float lastEttireuseTime;

// ─── Générateurs ─────────────────────────────────────────────────────────────
typedef struct Steam {
    int i, j;
    bool placed;
    int energy_q, energy_id;
    int material_q, material_id;
    int final_q;
    Texture2D texture;
} Steam;
extern Steam ListeSteam[MAX_STEAM];
extern int numSteams;
extern float lastSteamTime;

typedef struct Oil {
    int i, j;
    bool placed;
    int energy_q, energy_id;
    int material_q, material_id;
    int final_q;
    Texture2D texture;
} Oil;
extern Oil ListeOil[MAX_OIL];
extern int numOils;
extern float lastOilTime;

// ─── Convoyeur ────────────────────────────────────────────────────────────────
typedef struct Conveyor {
    int i, j;
    int dir[2];
    bool placed;
    bool power;
    int  load;
    int  peak_load;
    int  max_load;
    int  amount;
    int  capacity;
    bool processed;
    Texture2D texture;
    Texture2D textureToMove;
} Conveyor;
extern Conveyor ListeConveyor[MAX_CONVEYOR];

// ─── Batterie ─────────────────────────────────────────────────────────────────
typedef struct Battery {
    int i, j;
    int q;
    bool placed;
    Texture2D texture;
} Battery;
extern Battery ListeBattery[MAX_BATTERY];

// ─── Fonctions ───────────────────────────────────────────────────────────────
void InitInventoryKeyBiding(void);
void mouseDefault(void);
void leftClic(void);
void rightClic(void);
void UpdateDir(void);
void ActionWithName(char ObjectName[20], int i, int j, int option);
int  AddInInvent(int q, Texture2D texture);
const char* FindName(Texture2D textureRef);

#endif