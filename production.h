#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <stdbool.h>

// ─── Mise à jour des machines de production ───────────────────────────────────
void Update_Foreuse(void);
void Update_Furnace(void);
void Update_Hydraulic(void);
void Update_Ettireuse(void);
void Update_Steam(void);
void Update_Oil(void);

// ─── Détection de machine sur une case ───────────────────────────────────────
bool isForeuse(int posX, int posY);
bool isFurnace(int posX, int posY);
bool isHydraulic(int posX, int posY);
bool isEttireuse(int posX, int posY);
bool isSteam(int posX, int posY);
bool isOil(int posX, int posY);

// ─── Suppression ─────────────────────────────────────────────────────────────
void RemoveForeuse(int posX, int posY);
void RemoveFurnace(int posX, int posY);
void RemoveHydraulic(int posX, int posY);
void RemoveEttireuse(int posX, int posY);
void RemoveSteam(int posX, int posY);
void RemoveOil(int posX, int posY);

// ─── Interaction joueur ───────────────────────────────────────────────────────
void interraction(int posX, int posY);

#endif