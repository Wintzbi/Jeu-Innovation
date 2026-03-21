#include "energy.h"
#include "input.h"
#include "cell.h"
#include "texture.h"
#include <stdio.h>
 
// ─── Panneaux solaires — liste O(n) ──────────────────────────────────────────
#define MAX_SOLAR_PANELS 200
#define BFS_MAX 512

typedef struct { int i; int j; } SolarPanel;
static SolarPanel solarPanels[MAX_SOLAR_PANELS];
static int solarPanelCount = 0;
static int solarPool       = 0;

static int dbg_requests = 0;
static int dbg_success  = 0;

void AddSolarPanel(int i, int j) {
    if (solarPanelCount < MAX_SOLAR_PANELS)
        solarPanels[solarPanelCount++] = (SolarPanel){i, j};
}

void RemoveSolarPanel(int i, int j) {
    for (int s = 0; s < solarPanelCount; s++) {
        if (solarPanels[s].i == i && solarPanels[s].j == j) {
            solarPanels[s] = solarPanels[--solarPanelCount];
            return;
        }
    }
}

void RebuildSolarPanels(void) {
    solarPanelCount = 0;
    for (int i = 0; i < COL && solarPanelCount < MAX_SOLAR_PANELS; i++)
        for (int j = 0; j < ROW && solarPanelCount < MAX_SOLAR_PANELS; j++)
            if (grid[i][j].up_texture.id == solarpanelTexture.id)
                solarPanels[solarPanelCount++] = (SolarPanel){i, j};
}

// Appelé depuis Update_Conv() pour recharger le pool solaire
// Exposition du pool pour Update_Conv dans conveyor.c
int GetSolarPool(void)  { return solarPool; }
void SetSolarPool(int v){ solarPool = v; }
void ChargeSolarPool(int darkness) {
    int daylight = 64 - darkness;
    solarPool = (solarPanelCount * SOLAR_PER_PANEL * daylight) / 64;
}

// ─── HasEnergySource ─────────────────────────────────────────────────────────
int HasEnergySource(int x, int y, int range) {
    for (int i = -range; i <= range; i++) {
        for (int j = -range; j <= range; j++) {
            int nx = x + i, ny = y + j;
            if (!IndexIsValid(nx, ny)) continue;
            if (grid[nx][ny].up_texture.id == solarpanelTexture.id && solarPool > 0) return 1;
            if (grid[nx][ny].up_texture.id == piloneTexture.id &&
                grid[nx][ny].move_texture.id == piloneEffectTexture.id) return 1;
            for (int k = 0; k < numSteams; k++)
                if (ListeSteam[k].i == nx && ListeSteam[k].j == ny &&
                    ListeSteam[k].final_q > 0) return 1;
            for (int k = 0; k < MAX_BATTERY; k++)
                if (ListeBattery[k].placed && ListeBattery[k].q > 0 &&
                    ListeBattery[k].i == nx && ListeBattery[k].j == ny) return 1;
        }
    }
    return 0;
}

// ─── RequestEnergy (BFS) ─────────────────────────────────────────────────────
int RequestEnergy(int x, int y, int amount) {
    dbg_requests++;
    static int visitedI[BFS_MAX], visitedJ[BFS_MAX];
    static int parentIdx[BFS_MAX];
    int head = 0, tail = 0;

    visitedI[tail] = x; visitedJ[tail] = y; parentIdx[tail] = -1;
    tail++;

    while (head < tail) {
        int ci = visitedI[head], cj = visitedJ[head];
        head++;

        for (int di = -3; di <= 3; di++) {
            for (int dj = -3; dj <= 3; dj++) {
                int ni = ci + di, nj = cj + dj;
                if (!IndexIsValid(ni, nj)) continue;

                bool seen = false;
                for (int v = 0; v < tail; v++)
                    if (visitedI[v] == ni && visitedJ[v] == nj) { seen = true; break; }
                if (seen) continue;

                // Helper : remonter le chemin et incrémenter les pylônes
                #define BACKTRACK() do { \
                    int idx = head - 1; \
                    while (idx >= 0) { \
                        int pi = visitedI[idx], pj = visitedJ[idx]; \
                        for (int c = 0; c < MAX_CONVEYOR; c++) { \
                            if (ListeConveyor[c].placed && \
                                ListeConveyor[c].texture.id == piloneTexture.id && \
                                ListeConveyor[c].i == pi && ListeConveyor[c].j == pj) { \
                                ListeConveyor[c].load += amount; \
                                if (ListeConveyor[c].load > ListeConveyor[c].peak_load) \
                                    ListeConveyor[c].peak_load = ListeConveyor[c].load; \
                            } \
                        } \
                        idx = parentIdx[idx]; \
                    } \
                } while(0)

                // Panneau solaire
                if (grid[ni][nj].up_texture.id == solarpanelTexture.id &&
                    solarPool >= amount) {
                    solarPool -= amount;
                    BACKTRACK();
                    dbg_success++; return 1;
                }

                // Steam
                for (int k = 0; k < numSteams; k++) {
                    if (ListeSteam[k].i == ni && ListeSteam[k].j == nj &&
                        ListeSteam[k].final_q >= amount) {
                        ListeSteam[k].final_q -= amount;
                        BACKTRACK();
                        dbg_success++; return 1;
                    }
                }

                // Batterie
                if (FindNearestBattery(ni, nj)) {
                    BACKTRACK();
                    dbg_success++; return 1;
                }

                #undef BACKTRACK

                // Pylône : continuer BFS si capacité disponible
                for (int c = 0; c < MAX_CONVEYOR; c++) {
                    if (!ListeConveyor[c].placed) continue;
                    if (ListeConveyor[c].texture.id != piloneTexture.id) continue;
                    if (ListeConveyor[c].i != ni || ListeConveyor[c].j != nj) continue;
                    if (ListeConveyor[c].load + amount > ListeConveyor[c].max_load) continue;
                    if (tail < BFS_MAX) {
                        visitedI[tail] = ni; visitedJ[tail] = nj;
                        parentIdx[tail] = head - 1;
                        tail++;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}

// ─── FindNearestBattery / FindNearestSteam ───────────────────────────────────
int FindNearestBattery(int x, int y) {
    if (!IndexIsValid(x - 1, y - 1) || !IndexIsValid(x + 1, y + 1)) return 0;
    for (int k = 0; k < MAX_BATTERY; k++) {
        if (ListeBattery[k].placed &&
            ListeBattery[k].i >= x - 1 && ListeBattery[k].i <= x + 1 &&
            ListeBattery[k].j >= y - 1 && ListeBattery[k].j <= y + 1) {
            if (ListeBattery[k].q > 0) {
                ListeBattery[k].q--;
                return 1;
            }
        }
    }
    return 0;
}

int FindNearestSteam(int x, int y) {
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == x && ListeSteam[i].j == y && ListeSteam[i].final_q > 0) {
            ListeSteam[i].final_q--;
            return 1;
        }
    }
    return 0;
}

// ─── IsEnergieNear (ancienne API) ────────────────────────────────────────────
int IsEnergieNear(int x, int y, int range) {
    for (int i = -range; i <= range; i++) {
        for (int j = -range; j <= range; j++) {
            int nx = x + i, ny = y + j;
            if (!IndexIsValid(nx, ny)) continue;
            if (grid[nx][ny].up_texture.id == solarpanelTexture.id && solarPool > 0) return 1;
            if (grid[nx][ny].up_texture.id == piloneTexture.id &&
                grid[nx][ny].move_texture.id == piloneEffectTexture.id) return 1;
            if (FindNearestSteam(nx, ny))   return 1;
            if (FindNearestBattery(nx, ny)) return 1;
        }
    }
    return 0;
}

// ─── UpdateBattery ───────────────────────────────────────────────────────────
void UpdateBattery(void) {
    for (int k = 0; k < MAX_BATTERY; k++) {
        if (ListeBattery[k].placed && ListeBattery[k].q <= 100) {
            if (RequestEnergy(ListeBattery[k].i, ListeBattery[k].j, 1))
                ListeBattery[k].q++;
        }
    }
}

// ─── DebugEnergy (F1) ────────────────────────────────────────────────────────
void DebugEnergy(void) {
    printf("\n=== RÉSEAU ÉNERGÉTIQUE ===\n");
    int snap_req = dbg_requests, snap_ok = dbg_success;
    dbg_requests = dbg_success = 0;
    printf("  RequestEnergy : %d appels, %d succès (depuis dernier F1)\n", snap_req, snap_ok);

    int totalSteam = 0;
    for (int i = 0; i < numSteams; i++) {
        printf("  Steam  (%3d,%3d) : %3d énergie stockée\n",
               ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].final_q);
        totalSteam += ListeSteam[i].final_q;
    }
    printf("  Solaire : %3dA (%d panneaux × %dA)\n",
           solarPool, solarPanelCount, SOLAR_PER_PANEL);

    int piloneCount = 0;
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (!ListeConveyor[k].placed || ListeConveyor[k].texture.id != piloneTexture.id) continue;
        bool allume = grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture.id != 0;
        printf("  Pylône (%3d,%3d) : now=%d/5A  peak=%d/5A  [%s]\n",
               ListeConveyor[k].i, ListeConveyor[k].j,
               ListeConveyor[k].load, ListeConveyor[k].peak_load,
               allume ? "ALLUMÉ" : "éteint");
        ListeConveyor[k].peak_load = 0;
        piloneCount++;
    }
    if (!piloneCount) printf("  (aucun pylône posé)\n");

    printf("  Foreuses : %d\n", numForeuses);
    for (int k = 0; k < MAX_BATTERY; k++)
        if (ListeBattery[k].placed)
            printf("  Batterie  (%3d,%3d) : %d/100\n",
                   ListeBattery[k].i, ListeBattery[k].j, ListeBattery[k].q);
    for (int i = 0; i < numFurnaces; i++)
        printf("  Furnace   (%3d,%3d) : energy=%d mat=%d final=%d\n",
               ListeFurnace[i].i, ListeFurnace[i].j,
               ListeFurnace[i].energy_q, ListeFurnace[i].material_q, ListeFurnace[i].final_q);
    for (int i = 0; i < numHydraulics; i++)
        printf("  Hydraulic (%3d,%3d) : energy=%d mat=%d final=%d\n",
               ListeHydraulic[i].i, ListeHydraulic[i].j,
               ListeHydraulic[i].energy_q, ListeHydraulic[i].material_q, ListeHydraulic[i].final_q);
    for (int i = 0; i < numEttireuses; i++)
        printf("  Ettireuse (%3d,%3d) : energy=%d mat=%d final=%d\n",
               ListeEttireuse[i].i, ListeEttireuse[i].j,
               ListeEttireuse[i].energy_q, ListeEttireuse[i].material_q, ListeEttireuse[i].final_q);

    if (piloneCount > 0 && totalSteam > 0 && snap_req == 0)
        printf("  [!] Steam + pylônes OK mais 0 appels = machines sans timer\n");
    printf("=========================\n");
}
