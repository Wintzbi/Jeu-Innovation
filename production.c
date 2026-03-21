#include "production.h"
#include "input.h"
#include "energy.h"
#include "cell.h"
#include "texture.h"
#include "machine.h"
#include "craft.h"
#include "inventory.h"
#include "texture_registry.h"
#include <raylib.h>

// ─── Update_Foreuse ───────────────────────────────────────────────────────────
void Update_Foreuse(void) {
    float currentTime = GetTime();
    if (currentTime - lastForeuseTime >= 3.0f) {
        for (int i = 0; i < numForeuses; i++) {
            if (ListeForeuse[i].placed && IndexIsValid(ListeForeuse[i].i, ListeForeuse[i].j) &&
                RequestEnergy(ListeForeuse[i].i, ListeForeuse[i].j, 1)) {
                Texture2D tex = grid[ListeForeuse[i].i][ListeForeuse[i].j].texture;
                if (ListeForeuse[i].q < 100) {
                    if      (tex.id == copperVeinTexture.id) ListeForeuse[i].q += 2;
                    else if (tex.id == ironVeinTexture.id)   ListeForeuse[i].q += 2;
                    else if (tex.id == coalVeinTexture.id)   ListeForeuse[i].q += 3;
                    else if (tex.id == waterVeinTexture.id)  ListeForeuse[i].q += 2;
                    else if (tex.id == oilVeinTexture.id)    ListeForeuse[i].q += 2;
                    else if (tex.id == rockVeinTexture.id)   ListeForeuse[i].q += 2;
                    else if (tex.id == sandVeinTexture.id)   ListeForeuse[i].q += 3;
                }
            }
        }
        lastForeuseTime = GetTime();
    }
}

// ─── Update_Furnace / Hydraulic / Ettireuse ───────────────────────────────────
void Update_Furnace(void) {
    Update_Processor(ListeFurnace, numFurnaces,
                     &lastFurnaceTime, 4.0f, true,
                     furnaceEffect, FurnaceRecipes, FurnaceRecipeCount);
}

void Update_Hydraulic(void) {
    Update_Processor(ListeHydraulic, numHydraulics,
                     &lastHydraulicTime, 7.0f, true,
                     pressEffect, HydraulicRecipes, HydraulicRecipeCount);
}

void Update_Ettireuse(void) {
    Update_Processor(ListeEttireuse, numEttireuses,
                     &lastEttireuseTime, 8.0f, true,
                     stretchEffect, EttireuseRecipes, EttireuseRecipeCount);
}

// ─── Update_Steam ─────────────────────────────────────────────────────────────
void Update_Steam(void) {
    float currentTime = GetTime();
    if (currentTime - lastSteamTime >= 10.0f) {
        for (int i = 0; i < numSteams; i++) {
            if (!ListeSteam[i].placed || !IndexIsValid(ListeSteam[i].i, ListeSteam[i].j)) continue;
            if (ListeSteam[i].energy_q > 0 && ListeSteam[i].material_q > 0) {
                grid[ListeSteam[i].i][ListeSteam[i].j].move_texture = steamEffect;
                if (ListeSteam[i].material_id == waterVeinTexture.id &&
                    ListeSteam[i].final_q < 100) {
                    if (ListeSteam[i].energy_id == oilVeinTexture.id) {
                        ListeSteam[i].energy_q--;
                        ListeSteam[i].material_q--;
                        ListeSteam[i].final_q += 12;
                    } else if (ListeSteam[i].energy_id == coalTexture.id) {
                        ListeSteam[i].energy_q--;
                        ListeSteam[i].material_q--;
                        ListeSteam[i].final_q += 8;
                    }
                }
                if (ListeSteam[i].energy_q   == 0) ListeSteam[i].energy_id   = 0;
                if (ListeSteam[i].material_q  == 0) ListeSteam[i].material_id = 0;
            } else {
                grid[ListeSteam[i].i][ListeSteam[i].j].move_texture = (Texture2D){0};
            }
        }
        lastSteamTime = GetTime();
    }
}

// ─── Update_Oil ───────────────────────────────────────────────────────────────
void Update_Oil(void) {
    float currentTime = GetTime();
    if (currentTime - lastOilTime >= 20.0f) {
        for (int i = 0; i < numOils; i++) {
            if (!ListeOil[i].placed || !IndexIsValid(ListeOil[i].i, ListeOil[i].j)) continue;
            if (ListeOil[i].energy_q > 0 && ListeOil[i].material_q > 0 &&
                ListeOil[i].material_id == waterVeinTexture.id &&
                ListeOil[i].final_q < 50) {
                ListeOil[i].energy_q--;
                ListeOil[i].material_q--;
                ListeOil[i].final_q += 8;
                if (ListeOil[i].energy_q   == 0) ListeOil[i].energy_id   = 0;
                if (ListeOil[i].material_q  == 0) ListeOil[i].material_id = 0;
            }
        }
        lastOilTime = GetTime();
    }
}

// ─── is* ──────────────────────────────────────────────────────────────────────
bool isForeuse(int posX, int posY) {
    for (int i = 0; i < numForeuses; i++)
        if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) return true;
    return false;
}
bool isFurnace(int posX, int posY)   { return isMachine(ListeFurnace,   numFurnaces,   posX, posY); }
bool isHydraulic(int posX, int posY) { return isMachine(ListeHydraulic, numHydraulics, posX, posY); }
bool isEttireuse(int posX, int posY) { return isMachine(ListeEttireuse, numEttireuses, posX, posY); }

bool isSteam(int posX, int posY) {
    for (int i = 0; i < numSteams; i++)
        if (ListeSteam[i].i == posX && ListeSteam[i].j == posY) return true;
    return false;
}

bool isOil(int posX, int posY) {
    for (int i = 0; i < numOils; i++)
        if (ListeOil[i].i == posX && ListeOil[i].j == posY) return true;
    return false;
}

// ─── Remove* ──────────────────────────────────────────────────────────────────
void RemoveForeuse(int posX, int posY) {
    for (int i = 0; i < numForeuses; i++) {
        if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) {
            for (int j = i; j < numForeuses - 1; j++) ListeForeuse[j] = ListeForeuse[j + 1];
            numForeuses--;
            grid[posX][posY].move_texture = (Texture2D){0};
            return;
        }
    }
}
void RemoveFurnace(int posX, int posY)   { Remove_Machine(ListeFurnace,   &numFurnaces,   posX, posY); }
void RemoveHydraulic(int posX, int posY) { Remove_Machine(ListeHydraulic, &numHydraulics, posX, posY); }
void RemoveEttireuse(int posX, int posY) { Remove_Machine(ListeEttireuse, &numEttireuses, posX, posY); }

void RemoveSteam(int posX, int posY) {
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == posX && ListeSteam[i].j == posY) {
            for (int j = i; j < numSteams - 1; j++) ListeSteam[j] = ListeSteam[j + 1];
            numSteams--;
            grid[posX][posY].move_texture = (Texture2D){0};
            return;
        }
    }
}

void RemoveOil(int posX, int posY) {
    for (int i = 0; i < numOils; i++) {
        if (ListeOil[i].i == posX && ListeOil[i].j == posY) {
            for (int j = i; j < numOils - 1; j++) ListeOil[j] = ListeOil[j + 1];
            numOils--;
            grid[posX][posY].moveable = true;
            grid[posX][posY].move_texture = (Texture2D){0};
            return;
        }
    }
}

// ─── interraction ─────────────────────────────────────────────────────────────
static void interact_machine(Machine *m, unsigned int energy_tex_id,
                              unsigned int mat_tex_ids[]) {
    Item *sel = &inventory[selectedItem];
    unsigned int sel_id = sel->texture.id;

    if (sel->quantity <= 0) {
        if (m->final_q > 0 && m->final_id != 0) {
            Texture2D out = IdxToTex(TexToIdx((Texture2D){.id = m->final_id}));
            if (out.id == 0) out.id = m->final_id;
            AddInInvent(m->final_q, out);
            m->final_q = 0; m->final_id = 0;
        }
        return;
    }
    if (energy_tex_id != 0 && sel_id == energy_tex_id) {
        sel->quantity--;
        m->energy_q++;
        m->energy_id = energy_tex_id;
        return;
    }
    for (int t = 0; mat_tex_ids[t] != 0; t++) {
        if (sel_id == mat_tex_ids[t]) {
            if (m->material_id == 0 || m->material_id == sel_id) {
                sel->quantity--;
                m->material_q++;
                m->material_id = sel_id;
            }
            return;
        }
    }
    if (m->final_q > 0 && m->final_id != 0) {
        Texture2D out = IdxToTex(TexToIdx((Texture2D){.id = m->final_id}));
        if (out.id == 0) out.id = m->final_id;
        AddInInvent(m->final_q, out);
        m->final_q = 0; m->final_id = 0;
    }
}

static unsigned int FurnaceMats[]   = { 0, 0, 0, 0 };
static unsigned int HydraulicMats[] = { 0, 0, 0, 0 };
static unsigned int EttireuseMats[] = { 0, 0, 0, 0 };
static unsigned int SteamMats[]     = { 0, 0 };
static unsigned int OilMats[]       = { 0, 0 };
static bool matsReady = false;

static void InitInterractionMats(void) {
    if (matsReady) return;
    FurnaceMats[0]   = copperTexture.id;
    FurnaceMats[1]   = ironTexture.id;
    FurnaceMats[2]   = ironLingotTexture.id;
    HydraulicMats[0] = ironLingotTexture.id;
    HydraulicMats[1] = copperLingotTexture.id;
    HydraulicMats[2] = ironPlateTexture.id;
    EttireuseMats[0] = ironLingotTexture.id;
    EttireuseMats[1] = copperLingotTexture.id;
    EttireuseMats[2] = copperRodTexture.id;
    SteamMats[0]     = waterVeinTexture.id;
    OilMats[0]       = waterVeinTexture.id;
    matsReady = true;
}

void interraction(int posX, int posY) {
    InitInterractionMats();

    if (isForeuse(posX, posY)) {
        for (int i = 0; i < numForeuses; i++) {
            if (ListeForeuse[i].i != posX || ListeForeuse[i].j != posY) continue;
            Texture2D tex = grid[posX][posY].texture;
            if      (tex.id == copperVeinTexture.id) AddInInvent(ListeForeuse[i].q, copperTexture);
            else if (tex.id == ironVeinTexture.id)   AddInInvent(ListeForeuse[i].q, ironTexture);
            else if (tex.id == coalVeinTexture.id)   AddInInvent(ListeForeuse[i].q, coalTexture);
            ListeForeuse[i].q = 0;
        }
    } else if (isFurnace(posX, posY)) {
        for (int i = 0; i < numFurnaces; i++)
            if (ListeFurnace[i].i == posX && ListeFurnace[i].j == posY)
                interact_machine(&ListeFurnace[i], coalTexture.id, FurnaceMats);
    } else if (isHydraulic(posX, posY)) {
        for (int i = 0; i < numHydraulics; i++)
            if (ListeHydraulic[i].i == posX && ListeHydraulic[i].j == posY)
                interact_machine(&ListeHydraulic[i], coalTexture.id, HydraulicMats);
    } else if (isEttireuse(posX, posY)) {
        for (int i = 0; i < numEttireuses; i++)
            if (ListeEttireuse[i].i == posX && ListeEttireuse[i].j == posY)
                interact_machine(&ListeEttireuse[i], coalTexture.id, EttireuseMats);
    } else if (isSteam(posX, posY)) {
        for (int i = 0; i < numSteams; i++) {
            if (ListeSteam[i].i != posX || ListeSteam[i].j != posY) continue;
            Item *sel = &inventory[selectedItem];
            if (sel->quantity > 0 && sel->texture.id == coalTexture.id) {
                sel->quantity--;
                ListeSteam[i].energy_q++;
                ListeSteam[i].energy_id = coalTexture.id;
            } else if (sel->quantity > 0 && sel->texture.id == SteamMats[0]) {
                if (ListeSteam[i].material_id == 0 || ListeSteam[i].material_id == SteamMats[0]) {
                    sel->quantity--;
                    ListeSteam[i].material_q++;
                    ListeSteam[i].material_id = SteamMats[0];
                }
            }
        }
    } else if (isOil(posX, posY)) {
        for (int i = 0; i < numOils; i++) {
            if (ListeOil[i].i != posX || ListeOil[i].j != posY) continue;
            Item *sel = &inventory[selectedItem];
            if (sel->quantity > 0 && sel->texture.id == oilVeinTexture.id) {
                sel->quantity--;
                ListeOil[i].energy_q++;
                ListeOil[i].energy_id = oilVeinTexture.id;
            } else if (sel->quantity > 0 && sel->texture.id == OilMats[0]) {
                if (ListeOil[i].material_id == 0 || ListeOil[i].material_id == OilMats[0]) {
                    sel->quantity--;
                    ListeOil[i].material_q++;
                    ListeOil[i].material_id = OilMats[0];
                }
            }
        }
    }
}