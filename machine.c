#include "machine.h"
#include "cell.h"      // IndexIsValid, grid
#include "input.h"     // IsEnergieNear
#include "texture.h"   // toutes les textures

// ─── Tables de recettes ──────────────────────────────────────────────────────
// Chaque ligne : { material_id, output_id, energy_cost, output_qty, min_energy }
// min_energy == 0 → seule condition : energy_q > 0

MachineRecipe FurnaceRecipes[] = {
    // Cuivre brut  → lingot cuivre
    { 0, 0, 1, 1, 0 },  // IDs remplis dans InitMachineRecipes()
    // Fer brut     → lingot fer
    { 0, 0, 1, 1, 0 },
    // Lingot fer   → acier  (coûte 2 énergies, nécessite en avoir au moins 3)
    { 0, 0, 2, 1, 3 },
};
int FurnaceRecipeCount = 3;

MachineRecipe HydraulicRecipes[] = {
    // Lingot fer    → plaque fer
    { 0, 0, 1, 1, 0 },
    // Lingot cuivre → plaque cuivre
    { 0, 0, 1, 1, 0 },
    // Plaque fer    → engrenage
    { 0, 0, 1, 1, 0 },
};
int HydraulicRecipeCount = 3;

MachineRecipe EttireuseRecipes[] = {
    // Lingot fer    → barre fer
    { 0, 0, 1, 1, 0 },
    // Lingot cuivre → barre cuivre
    { 0, 0, 1, 1, 0 },
    // Barre cuivre  → câble cuivre
    { 0, 0, 1, 1, 0 },
};
int EttireuseRecipeCount = 3;

// Les IDs de texture ne sont disponibles qu'après InitTexture(), donc on
// initialise les recettes au premier appel via un flag.
static bool recipesReady = false;

static void InitMachineRecipes(void) {
    if (recipesReady) return;

    MachineRecipe *f = FurnaceRecipes;
    f[0].material_id = copperTexture.id;      f[0].output_id = copperLingotTexture.id;
    f[1].material_id = ironTexture.id;        f[1].output_id = ironLingotTexture.id;
    f[2].material_id = ironLingotTexture.id;  f[2].output_id = steelTexture.id;

    MachineRecipe *h = HydraulicRecipes;
    h[0].material_id = ironLingotTexture.id;  h[0].output_id = ironPlateTexture.id;
    h[1].material_id = copperLingotTexture.id;h[1].output_id = copperPlateTexture.id;
    h[2].material_id = ironPlateTexture.id;   h[2].output_id = gearTexture.id;

    MachineRecipe *e = EttireuseRecipes;
    e[0].material_id = ironLingotTexture.id;  e[0].output_id = ironRodTexture.id;
    e[1].material_id = copperLingotTexture.id;e[1].output_id = copperRodTexture.id;
    e[2].material_id = copperRodTexture.id;   e[2].output_id = copperCableTexture.id;

    recipesReady = true;
}

// ─── Fonctions génériques ────────────────────────────────────────────────────

bool isMachine(Machine *list, int count, int posX, int posY) {
    for (int i = 0; i < count; i++) {
        if (list[i].i == posX && list[i].j == posY)
            return true;
    }
    return false;
}

void Remove_Machine(Machine *list, int *count, int posX, int posY) {
    for (int i = 0; i < *count; i++) {
        if (list[i].i == posX && list[i].j == posY) {
            // Compactage : décale les éléments suivants
            for (int j = i; j < *count - 1; j++)
                list[j] = list[j + 1];
            (*count)--;
            grid[posX][posY].move_texture = (Texture2D){0};
            return;
        }
    }
}

void Update_Processor(Machine *list, int count,
                      float *lastTime, float interval, bool checkTimer,
                      Texture2D effect,
                      const MachineRecipe *recipes, int recipeCount) {
    InitMachineRecipes();

    float currentTime = GetTime();
    if (checkTimer && (currentTime - *lastTime < interval))
        return;

    for (int i = 0; i < count; i++) {
        Machine *m = &list[i];
        if (!m->placed || !IndexIsValid(m->i, m->j))
            continue;

        // Charge de l'énergie si une source est accessible (consomme 1A du réseau)
        if (RequestEnergy(m->i, m->j, 1))
            m->energy_q++;

        // Effet visuel selon présence d'énergie
        grid[m->i][m->j].move_texture = (m->energy_q > 0) ? effect : (Texture2D){0};

        // Production : cherche une recette compatible
        if (m->energy_q <= 0 || m->material_q <= 0)
            continue;

        for (int r = 0; r < recipeCount; r++) {
            const MachineRecipe *rec = &recipes[r];

            // Vérifie matériau + seuil d'énergie minimum
            if (m->material_id != rec->material_id)
                continue;
            if (rec->min_energy > 0 && m->energy_q <= rec->min_energy)
                continue;

            m->energy_q   -= rec->energy_cost;
            m->material_q -= 1;
            m->final_q    += rec->output_qty;
            m->final_id    = rec->output_id;

            if (m->energy_q   == 0) m->energy_id   = 0;
            if (m->material_q == 0) m->material_id  = 0;
            break;  // Une seule recette par tick
        }
    }

    if (checkTimer)
        *lastTime = currentTime;
}