#ifndef MACHINE_H
#define MACHINE_H

#include "raylib.h"
#include <stdbool.h>

// Struct unifiée pour Furnace, Hydraulic, Ettireuse (processeurs)
// Steam et Oil gardent leur propre struct (générateurs, logique différente)
typedef struct Machine {
    int i, j;
    bool placed;
    int energy_q, energy_id;
    int material_q, material_id;
    int final_q,   final_id;
    Texture2D texture;
} Machine;

// Recette de transformation : un matériau → un produit
// min_energy : énergie minimale requise avant de produire (0 = juste > 0)
typedef struct MachineRecipe {
    int material_id;
    int output_id;
    int energy_cost;   // Énergie consommée par production
    int output_qty;    // Quantité produite
    int min_energy;    // Seuil minimum d'énergie requis (0 = aucun seuil)
} MachineRecipe;

// ─── Recettes Furnace ────────────────────────────────────────────────────────
// Définies dans machine.c, exposées pour pouvoir être testées
extern MachineRecipe FurnaceRecipes[];
extern int FurnaceRecipeCount;

// ─── Recettes Hydraulic ──────────────────────────────────────────────────────
extern MachineRecipe HydraulicRecipes[];
extern int HydraulicRecipeCount;

// ─── Recettes Ettireuse ──────────────────────────────────────────────────────
extern MachineRecipe EttireuseRecipes[];
extern int EttireuseRecipeCount;

// ─── API générique ───────────────────────────────────────────────────────────

// Retourne true si une machine est posée à (posX, posY)
bool isMachine(Machine *list, int count, int posX, int posY);

// Supprime la machine à (posX, posY) en compactant le tableau
void Remove_Machine(Machine *list, int *count, int posX, int posY);

// Met à jour tous les processeurs d'une liste (Furnace / Hydraulic / Ettireuse)
//   list        : tableau de machines
//   count       : nombre de machines actives
//   lastTime    : pointeur sur le timestamp du dernier tick
//   interval    : intervalle entre deux ticks (secondes)
//   effect      : texture d'effet quand la machine est active
//   recipes     : table de recettes
//   recipeCount : taille de la table
//   checkTimer  : si false, tourne chaque frame (Hydraulic, Ettireuse)
void Update_Processor(Machine *list, int count,
                      float *lastTime, float interval, bool checkTimer,
                      Texture2D effect,
                      const MachineRecipe *recipes, int recipeCount);

#endif