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
extern float rotation;  // Angle du conveyor sélectionné (mis à jour par UpdateDir)
extern int conveyor_dir;
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

// ─── Foreuse (cas particulier : pas de transformation) ───────────────────────
typedef struct Foreuse {
    int i, j;
    bool placed;
    int q;
    Texture2D texture;
} Foreuse;
extern Foreuse ListeForeuse[MAX_FOREUSE];
extern int numForeuses;
void Update_Foreuse();

// ─── Machines processeurs (Furnace, Hydraulic, Ettireuse) ────────────────────
// Struct unifiée — voir machine.h pour les détails
#include "machine.h"

extern Machine ListeFurnace[MAX_FURNACE];
extern int numFurnaces;
void Update_Furnace();

extern Machine ListeHydraulic[MAX_HYDRAULIC];
extern int numHydraulics;
void Update_Hydraulic();

extern Machine ListeEttireuse[MAX_ETTIREUSE];
extern int numEttireuses;
void Update_Ettireuse();

// ─── Générateurs d'énergie (Steam, Oil) ──────────────────────────────────────
// Logique différente des processeurs : produisent de l'énergie (final_q),
// pas de matériau de sortie → struct séparée (sans final_id)
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
void Update_Steam();
int FindNearestSteam(int x, int y);

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
void Update_Oil();

// Conveyor
typedef struct Conveyor {
    int i;
    int j;
    int dir[2];
    bool placed;
    bool power;
    int  load;       // ampères transportés ce tick
    int  peak_load;  // max vu depuis le dernier reset manuel (debug)
    int  max_load;   // capacité max du pylône (0 pour convoyeurs/tuyaux)
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
void RemoveForeuse(int posX, int posY);
void RemoveFurnace(int posX, int posY);
void RemoveHydraulic(int posX, int posY);
void RemoveEttireuse(int posX, int posY);
void RemoveSteam(int posX, int posY);
void RemoveOil(int posX, int posY);

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
int HasEnergySource(int x, int y, int range);
int RequestEnergy(int x, int y, int amount);
void DebugEnergy(void);

// Détection de machine sur une case
bool isForeuse(int posX, int posY);
bool isFurnace(int posX, int posY);
bool isHydraulic(int posX, int posY);
bool isEttireuse(int posX, int posY);
bool isSteam(int posX, int posY);
bool isOil(int posX, int posY);

#endif