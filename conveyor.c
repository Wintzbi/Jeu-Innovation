#include "conveyor.h"
#include "energy.h"
#include "cell.h"
#include "texture.h"
#include "machine.h"
#include "inventory.h"
#include "texture_registry.h"
#include <raylib.h>

// Caps de stockage — dos de tapis si plein
#define CAP_ENERGY        20
#define CAP_MATERIAL      10
#define CAP_STEAM_ENERGY  30
#define CAP_STEAM_MATERIAL 20

// ─── Update_Conv ─────────────────────────────────────────────────────────────
void Update_Conv(void) {
    // Étape 1 : reset pylônes + recharge solaire
    ChargeSolarPool(DayAndNight());
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (!ListeConveyor[k].placed || ListeConveyor[k].texture.id != piloneTexture.id) continue;
        ListeConveyor[k].load = 0;
        ListeConveyor[k].textureToMove = (Texture2D){0};
        grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture = (Texture2D){0};
    }

    // Étape 2 : allumer les pylônes (multi-pass)
    bool changed = true;
    while (changed) {
        changed = false;
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) continue;
            if (ListeConveyor[k].texture.id != piloneTexture.id) continue;
            if (grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture.id != 0) continue;
            if (HasEnergySource(ListeConveyor[k].i, ListeConveyor[k].j, 3)) {
                grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture = piloneEffectTexture;
                ListeConveyor[k].textureToMove = piloneEffectTexture;
                changed = true;
            }
        }
    }

    // Étape 3 : reset processed + déplacer items
    for (int k = 0; k < MAX_CONVEYOR; k++)
        ListeConveyor[k].processed = false;
    for (int k = 0; k < MAX_CONVEYOR; k++)
        if (ListeConveyor[k].placed && ListeConveyor[k].texture.id != piloneTexture.id)
            Convey(&ListeConveyor[k]);
}

// ─── Convey ──────────────────────────────────────────────────────────────────
void Convey(Conveyor *conv) {
    if (conv->texture.id == piloneTexture.id) return;
    if (conv->processed) return;
    conv->processed = true;

    int srcI  = conv->i - conv->dir[0];
    int srcJ  = conv->j - conv->dir[1];
    int destI = conv->i + conv->dir[0];
    int destJ = conv->j + conv->dir[1];

    if (!IndexIsValid(srcI, srcJ) || !IndexIsValid(destI, destJ)) return;

    // Mode push si le convoyeur devant a une direction différente
    bool pushMode = false;
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (!ListeConveyor[k].placed) continue;
        if (ListeConveyor[k].i != destI || ListeConveyor[k].j != destJ) continue;
        if (ListeConveyor[k].texture.id != conveyorTexture.id &&
            ListeConveyor[k].texture.id != pipeTexture.id) break;
        if (ListeConveyor[k].dir[0] != conv->dir[0] ||
            ListeConveyor[k].dir[1] != conv->dir[1])
            pushMode = true;
        break;
    }

    // ── Pull : prendre depuis la source ──────────────────────────────────
    if (conv->amount == 0) {
        bool srcIsConv = (grid[srcI][srcJ].up_texture.id == conveyorTexture.id ||
                          grid[srcI][srcJ].up_texture.id == pipeTexture.id);

        if (srcIsConv && grid[srcI][srcJ].move_texture.id != 0) {
            for (int k = 0; k < MAX_CONVEYOR; k++) {
                if (!ListeConveyor[k].placed) continue;
                if (ListeConveyor[k].i != srcI || ListeConveyor[k].j != srcJ) continue;
                if (ListeConveyor[k].amount == 0) continue;
                int take = ListeConveyor[k].amount < conv->capacity
                         ? ListeConveyor[k].amount : conv->capacity;
                conv->textureToMove        = ListeConveyor[k].textureToMove;
                conv->amount               = take;
                ListeConveyor[k].amount   -= take;
                ListeConveyor[k].processed = true;
                if (ListeConveyor[k].amount == 0)
                    ListeConveyor[k].textureToMove = (Texture2D){0};
                grid[srcI][srcJ].move_texture = ListeConveyor[k].amount > 0
                    ? ListeConveyor[k].textureToMove : (Texture2D){0};
                break;
            }
        }
        else if (grid[srcI][srcJ].up_texture.id == drillTexture.id) {
            for (int k = 0; k < numForeuses; k++) {
                if (ListeForeuse[k].i != srcI || ListeForeuse[k].j != srcJ) continue;
                if (ListeForeuse[k].q <= 0) continue;
                Texture2D under = grid[srcI][srcJ].texture;
                Texture2D mined = (Texture2D){0};
                if (conv->texture.id == conveyorTexture.id) {
                    if      (under.id == copperVeinTexture.id) mined = copperTexture;
                    else if (under.id == ironVeinTexture.id)   mined = ironTexture;
                    else if (under.id == coalVeinTexture.id)   mined = coalTexture;
                    else if (under.id == rockVeinTexture.id)   mined = rockTexture;
                    else if (under.id == sandVeinTexture.id)   mined = sandDust;
                } else if (conv->texture.id == pipeTexture.id) {
                    if      (under.id == waterVeinTexture.id)  mined = waterVeinTexture;
                    else if (under.id == oilVeinTexture.id)    mined = oilVeinTexture;
                }
                if (mined.id != 0) {
                    int take = ListeForeuse[k].q < conv->capacity
                             ? ListeForeuse[k].q : conv->capacity;
                    conv->textureToMove = mined;
                    conv->amount        = take;
                    ListeForeuse[k].q  -= take;
                }
                break;
            }
        }
        else {
            Machine *src_machine = NULL; int src_count = 0;
            if      (grid[srcI][srcJ].up_texture.id == furnaceTexture.id)
                { src_machine = ListeFurnace;   src_count = numFurnaces;   }
            else if (grid[srcI][srcJ].up_texture.id == pressTexture.id)
                { src_machine = ListeHydraulic; src_count = numHydraulics; }
            else if (grid[srcI][srcJ].up_texture.id == stretchTexture.id)
                { src_machine = ListeEttireuse; src_count = numEttireuses; }
            if (src_machine) {
                for (int k = 0; k < src_count; k++) {
                    if (src_machine[k].i != srcI || src_machine[k].j != srcJ) continue;
                    if (src_machine[k].final_q <= 0) continue;
                    Texture2D out = IdxToTex(TexToIdx((Texture2D){.id = src_machine[k].final_id}));
                    if (out.id == 0) out.id = src_machine[k].final_id;
                    int take = src_machine[k].final_q < conv->capacity
                             ? src_machine[k].final_q : conv->capacity;
                    conv->textureToMove    = out;
                    conv->amount           = take;
                    src_machine[k].final_q -= take;
                    break;
                }
            }
        }

        grid[conv->i][conv->j].move_texture = conv->amount > 0
            ? conv->textureToMove : (Texture2D){0};

        if (!pushMode) return;
    }

    // ── Push : injecter sur le tapis devant (merge) ───────────────────────
    if (pushMode && conv->amount > 0) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) continue;
            if (ListeConveyor[k].i != destI || ListeConveyor[k].j != destJ) continue;
            if (ListeConveyor[k].processed) break;
            if (ListeConveyor[k].amount > 0) break;
            int take = conv->amount < ListeConveyor[k].capacity
                     ? conv->amount : ListeConveyor[k].capacity;
            ListeConveyor[k].textureToMove = conv->textureToMove;
            ListeConveyor[k].amount        = take;
            conv->amount                  -= take;
            if (conv->amount == 0) conv->textureToMove = (Texture2D){0};
            grid[conv->i][conv->j].move_texture = conv->amount > 0
                ? conv->textureToMove : (Texture2D){0};
            grid[destI][destJ].move_texture = ListeConveyor[k].textureToMove;
            break;
        }
        return;
    }

    // ── Livrer à la destination ───────────────────────────────────────────
    if (conv->amount <= 0) return;
    unsigned int dtex = grid[destI][destJ].up_texture.id;

    if (dtex == chestTexture.id) {
        AddInInvent(conv->amount, conv->textureToMove);
        conv->amount = 0;
        conv->textureToMove = (Texture2D){0};
        grid[conv->i][conv->j].move_texture = (Texture2D){0};
        return;
    }

    // Livraison machine processeur
    Machine *dst = NULL; int dst_n = 0;
    if      (dtex == furnaceTexture.id)  { dst = ListeFurnace;   dst_n = numFurnaces;   }
    else if (dtex == pressTexture.id)    { dst = ListeHydraulic; dst_n = numHydraulics; }
    else if (dtex == stretchTexture.id)  { dst = ListeEttireuse; dst_n = numEttireuses; }

    if (dst) {
        for (int k = 0; k < dst_n; k++) {
            if (dst[k].i != destI || dst[k].j != destJ) continue;
            unsigned int tid = conv->textureToMove.id;
            bool is_nrj = (tid == coalTexture.id);
            if (is_nrj) {
                if ((dst[k].energy_id == tid || dst[k].energy_q == 0)
                    && dst[k].energy_q < CAP_ENERGY) {
                    int take = conv->amount < CAP_ENERGY - dst[k].energy_q
                             ? conv->amount : CAP_ENERGY - dst[k].energy_q;
                    dst[k].energy_id  = tid;
                    dst[k].energy_q  += take;
                    conv->amount -= take;
                    if (!conv->amount) {
                        conv->textureToMove = (Texture2D){0};
                        grid[conv->i][conv->j].move_texture = (Texture2D){0};
                    }
                }
            } else {
                if ((dst[k].material_id == tid || dst[k].material_q == 0)
                    && dst[k].material_q < CAP_MATERIAL) {
                    int take = conv->amount < CAP_MATERIAL - dst[k].material_q
                             ? conv->amount : CAP_MATERIAL - dst[k].material_q;
                    dst[k].material_id  = tid;
                    dst[k].material_q  += take;
                    conv->amount -= take;
                    if (!conv->amount) {
                        conv->textureToMove = (Texture2D){0};
                        grid[conv->i][conv->j].move_texture = (Texture2D){0};
                    }
                }
            }
            break;
        }
        return;
    }

    // Livraison centrale vapeur
    if (dtex == steamcentralTexture.id) {
        for (int k = 0; k < numSteams; k++) {
            if (ListeSteam[k].i != destI || ListeSteam[k].j != destJ) continue;
            unsigned int tid = conv->textureToMove.id;
            bool is_fuel = (tid == coalTexture.id || tid == oilVeinTexture.id);
            if (is_fuel) {
                if ((ListeSteam[k].energy_id == tid || ListeSteam[k].energy_q == 0)
                    && ListeSteam[k].energy_q < CAP_STEAM_ENERGY) {
                    int take = conv->amount < CAP_STEAM_ENERGY - ListeSteam[k].energy_q
                             ? conv->amount : CAP_STEAM_ENERGY - ListeSteam[k].energy_q;
                    ListeSteam[k].energy_id = tid;
                    ListeSteam[k].energy_q += take;
                    conv->amount -= take;
                    if (!conv->amount) {
                        conv->textureToMove = (Texture2D){0};
                        grid[conv->i][conv->j].move_texture = (Texture2D){0};
                    }
                }
            } else {
                if ((ListeSteam[k].material_id == tid || ListeSteam[k].material_q == 0)
                    && ListeSteam[k].material_q < CAP_STEAM_MATERIAL) {
                    int take = conv->amount < CAP_STEAM_MATERIAL - ListeSteam[k].material_q
                             ? conv->amount : CAP_STEAM_MATERIAL - ListeSteam[k].material_q;
                    ListeSteam[k].material_id = tid;
                    ListeSteam[k].material_q += take;
                    conv->amount -= take;
                    if (!conv->amount) {
                        conv->textureToMove = (Texture2D){0};
                        grid[conv->i][conv->j].move_texture = (Texture2D){0};
                    }
                }
            }
            break;
        }
    }
}

// ─── Remove* ─────────────────────────────────────────────────────────────────
void RemoveConveyor(int posX, int posY) {
    for (int i = 0; i < MAX_CONVEYOR; i++) {
        if (ListeConveyor[i].i == posX && ListeConveyor[i].j == posY) {
            for (int j = i; j < MAX_CONVEYOR - 1; j++)
                ListeConveyor[j] = ListeConveyor[j + 1];
            grid[posX][posY].moveable = true;
            grid[posX][posY].move_texture = (Texture2D){0};
            return;
        }
    }
}

void RemoveBattery(int posX, int posY) {
    for (int i = 0; i < MAX_BATTERY; i++) {
        if (ListeBattery[i].i == posX && ListeBattery[i].j == posY) {
            for (int j = i; j < MAX_BATTERY - 1; j++)
                ListeBattery[j] = ListeBattery[j + 1];
            grid[posX][posY].moveable = true;
            grid[posX][posY].move_texture = (Texture2D){0};
            return;
        }
    }
}
