#include "cell.h"
#include "input.h"
#include "inventory.h"
#include "camera.h"
#include "texture_registry.h"
#include <stdio.h>
#include <string.h> // Pour strcmp

int MinPlaceableID = 23;
float rotation = 0.0f;  // Angle du conveyor sélectionné
Conveyor ListeConveyor[MAX_CONVEYOR];
Foreuse ListeForeuse[MAX_FOREUSE];
int conveyor_dir=0;
int option =0;
int numForeuses = 0;
float lastForeuseTime;

Machine ListeFurnace[MAX_FURNACE];
int numFurnaces = 0;
float lastFurnaceTime;

Machine ListeHydraulic[MAX_HYDRAULIC];
int numHydraulics = 0;
float lastHydraulicTime;

Machine ListeEttireuse[MAX_ETTIREUSE];
int numEttireuses = 0;
float lastEttireuseTime;

Steam ListeSteam[MAX_STEAM];
int numSteams = 0;  // Compteur de foreuses
float lastSteamTime;

Oil ListeOil[MAX_OIL];
int numOils = 0;  // Compteur de foreuses
float lastOilTime;

Battery ListeBattery[MAX_BATTERY];

int directions[4][2] = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1} 
    };

void mouseDefault() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (inventory[selectedItem].texture.id != 0 && inventory[selectedItem].texture.id >= MinPlaceableID) {
        Rectangle destRect = { posX * cellSize + cellSize / 2, posY * cellSize + cellSize / 2, cellSize, cellSize };
        Rectangle srcRect = { 0, 0, inventory[selectedItem].texture.width, inventory[selectedItem].texture.height };
        Vector2 origin = { cellSize / 2, cellSize / 2 };

        if (inventory[selectedItem].texture.id == conveyorTexture.id || inventory[selectedItem].texture.id == pipeTexture.id || inventory[selectedItem].texture.id == piloneTexture.id ){
            DrawTexturePro(inventory[selectedItem].texture, srcRect, destRect, origin, rotation, Fade(WHITE, 0.35f));
        }

        else {
            DrawTexturePro(inventory[selectedItem].texture, srcRect, destRect, origin, 0.0f, Fade(WHITE, 0.35f));
        }
    }
    else DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(RED, 0.35f));
}
    
void InitInventoryKeyBiding() {
    if (IsKeyPressed(KEY_ONE)) selectedItem = 0;
    if (IsKeyPressed(KEY_TWO)) selectedItem = 1;
    if (IsKeyPressed(KEY_THREE)) selectedItem = 2;
    if (IsKeyPressed(KEY_FOUR)) selectedItem = 3;
    if (IsKeyPressed(KEY_FIVE)) selectedItem = 4;
    if (IsKeyPressed(KEY_SIX)) selectedItem = 5;
    if (IsKeyPressed(KEY_SEVEN)) selectedItem = 6;
    if (IsKeyPressed(KEY_EIGHT)) selectedItem = 7;
    if (IsKeyPressed(KEY_NINE)) selectedItem = 8;
    if (IsKeyPressed(KEY_ZERO)) selectedItem = 9;
}

void interraction(int posX, int posY);

void rightClic() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {

        interraction(posX,posY);
        
        if (inventory[selectedItem].quantity > 0 &&
                   inventory[selectedItem].texture.id >= MinPlaceableID) {
            if (IndexIsValid(posX, posY) && !grid[posX][posY].placed) {
                grid[posX][posY].placed = true;
                grid[posX][posY].up_texture = inventory[selectedItem].texture;
                ActionWithName(inventory[selectedItem].name, posX, posY,option);
                inventory[selectedItem].quantity--;

                if (inventory[selectedItem].quantity == 0) {
                    inventory[selectedItem].texture = (Texture2D){0};
                }
            }
        }
    }
}

void RemoveForeuse(int posX, int posY);
void RemoveFurnace(int posX, int posY);
void RemoveHydraulic(int posX, int posY);
void RemoveEttireuse(int posX, int posY);
void RemoveSteam(int posX, int posY);
void RemoveOil(int posX, int posY);

void leftClic() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (IndexIsValid(posX, posY) && grid[posX][posY].placed && grid[posX][posY].pickable &&
            grid[posX][posY].up_texture.id != 0) {
            Texture2D brokenTexture = grid[posX][posY].up_texture;
            bool itemFoundInInventory = false;

            for (int i = 0; i < INVENTORY_SIZE; i++) {
                if (inventory[i].texture.id == brokenTexture.id) {
                    inventory[i].quantity++;
                    itemFoundInInventory = true;
                    break;
                }
            }

            if (!itemFoundInInventory) {
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i].quantity == 0) {
                        inventory[i].texture = brokenTexture;
                        inventory[i].quantity = 1;
                        break;
                    }
                }
            }

            grid[posX][posY].up_texture = (Texture2D){0};
            grid[posX][posY].placed = false;
            grid[posX][posY].dir[0] = 0;
            grid[posX][posY].dir[1] = 0;
            pickedObject++;

            for (int f = 0; f < numForeuses; f++) {
                if (ListeForeuse[f].i == posX && ListeForeuse[f].j == posY && ListeForeuse[f].placed) {
                    ListeForeuse[f].placed = false;
                    RemoveForeuse(ListeForeuse[f].i, ListeForeuse[f].j);
                    grid[posX][posY].moveable = true;
                    break;
                }
            }
            for (int f = 0; f < MAX_BATTERY; f++) {
                if (ListeBattery[f].i == posX && ListeBattery[f].j == posY && ListeBattery[f].placed) {
                    ListeBattery[f].placed = false;
                    RemoveBattery(ListeBattery[f].i, ListeBattery[f].j);
                    break;
                }
            }
            for (int f = 0; f < MAX_CONVEYOR; f++) {
                if (ListeConveyor[f].i == posX && ListeConveyor[f].j == posY && ListeConveyor[f].placed) {
                    ListeConveyor[f].placed = false;
                    grid[posX][posY].dir[0] = 0;
                    grid[posX][posY].dir[1] = 0;
                    grid[posX][posY].move_texture = (Texture2D){0};
                    RemoveConveyor(ListeConveyor[f].i, ListeConveyor[f].j);
                    break;
                }
            }
            // Machines processeurs
            if (isMachine(ListeFurnace, numFurnaces, posX, posY)) {
                RemoveFurnace(posX, posY);
                grid[posX][posY].moveable = true;
            }
            if (isMachine(ListeHydraulic, numHydraulics, posX, posY)) {
                RemoveHydraulic(posX, posY);
                grid[posX][posY].moveable = true;
            }
            if (isMachine(ListeEttireuse, numEttireuses, posX, posY)) {
                RemoveEttireuse(posX, posY);
                grid[posX][posY].moveable = true;
            }
            // Générateurs
            for (int f = 0; f < numSteams; f++) {
                if (ListeSteam[f].i == posX && ListeSteam[f].j == posY && ListeSteam[f].placed) {
                    RemoveSteam(posX, posY);
                    grid[posX][posY].moveable = true;
                    break;
                }
            }
            for (int f = 0; f < numOils; f++) {
                if (ListeOil[f].i == posX && ListeOil[f].j == posY && ListeOil[f].placed) {
                    RemoveOil(posX, posY);
                    grid[posX][posY].moveable = true;
                    break;
                }
            }
            
        }
    }
}
void UpdateDir(){
    conveyor_dir = (conveyor_dir + 1) % 4;
    rotation = (conveyor_dir * 90);
}

void ActionWithName(char ObjectName[20], int i, int j, int option) {
    (void)ObjectName; // conservé pour compatibilité save/load nom-based
    unsigned int id = inventory[selectedItem].texture.id;

    // Convoyeurs — détectés par texture
    if (id == conveyorTexture.id || id == pipeTexture.id || id == piloneTexture.id) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                grid[i][j].dir[0] = directions[conveyor_dir][0];
                grid[i][j].dir[1] = directions[conveyor_dir][1];
                Texture2D tex = (id == conveyorTexture.id) ? conveyorTexture
                              : (id == pipeTexture.id)     ? pipeTexture
                                                           : piloneTexture;
                int mload = (id == piloneTexture.id) ? 5 : 0;
                ListeConveyor[k] = (Conveyor){
                    .i = i, .j = j, .texture = tex, .max_load = mload,
                    .dir = {directions[conveyor_dir][0], directions[conveyor_dir][1]},
                    .placed = true, .inMouvement = false, .textureToMove = (Texture2D){0}
                };
                if (id == pipeTexture.id) grid[i][j].moveable = false;
                break;
            }
        }
    } else if (id == batteryTexture.id) {
        for (int k = 0; k < MAX_BATTERY; k++) {
            if (!ListeBattery[k].placed) {
                ListeBattery[k] = (Battery){.i = i, .j = j, .texture = batteryTexture, .q = 0, .placed = true};
                break;
            }
        }
    } else if (id == drillTexture.id) {
        if (numForeuses < MAX_FOREUSE) {
            ListeForeuse[numForeuses++] = (Foreuse){.i = i, .j = j, .q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (id == furnaceTexture.id) {
        if (numFurnaces < MAX_FURNACE) {
            ListeFurnace[numFurnaces++] = (Machine){.i = i, .j = j, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (id == pressTexture.id) {
        if (numHydraulics < MAX_HYDRAULIC) {
            ListeHydraulic[numHydraulics++] = (Machine){.i = i, .j = j, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (id == stretchTexture.id) {
        if (numEttireuses < MAX_ETTIREUSE) {
            ListeEttireuse[numEttireuses++] = (Machine){.i = i, .j = j, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (id == steamcentralTexture.id) {
        if (numSteams < MAX_STEAM) {
            ListeSteam[numSteams++] = (Steam){.i = i, .j = j, .placed = true};
            grid[i][j].moveable = false;
        }
    }
    // Centrale Pétrole : pas de texture dédiée dans texture.h pour l'instant
    // (strcmp conservé comme fallback)
    else if (strcmp(ObjectName, "Centrale Pétrole") == 0) {
        if (numOils < MAX_OIL) {
            ListeOil[numOils++] = (Oil){.i = i, .j = j, .placed = true};
            grid[i][j].moveable = false;
        }
    }
}

// ─── Système énergétique ─────────────────────────────────────────────────────
// BFS depuis (startX, startY) pour trouver un chemin vers une source d'énergie.
// Chaque pylône sur le chemin doit avoir load < max_load.
// Si un chemin valide est trouvé : décrémente la source, incrémente les pylônes.
// Retourne 1 si l'énergie est accordée, 0 sinon.

#define BFS_MAX 512

// Vérifie la présence d'énergie SANS consommer (pour allumer les pylônes)
// Compteurs de diagnostic — remis à 0 par DebugEnergy()
static int dbg_requests = 0;
static int dbg_success  = 0;

// Pool d'énergie solaire — rechargé chaque tick dans Update_Conv
// Chaque panneau solaire contribue SOLAR_PER_PANEL unités/tick
#define SOLAR_PER_PANEL 3
static int solarPool = 0;

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

int RequestEnergy(int x, int y, int amount) {
    dbg_requests++;
    // BFS : tableau de cellules visitées + parent pour reconstruire le chemin
    static int visitedI[BFS_MAX], visitedJ[BFS_MAX];
    static int parentIdx[BFS_MAX];  // index du parent dans visited[]
    int head = 0, tail = 0;

    visitedI[tail] = x; visitedJ[tail] = y; parentIdx[tail] = -1;
    tail++;

    while (head < tail) {
        int ci = visitedI[head], cj = visitedJ[head];
        head++;

        // Chercher dans un rayon de 3 cases (portée pylône)
        for (int di = -3; di <= 3; di++) {
            for (int dj = -3; dj <= 3; dj++) {
                int ni = ci + di, nj = cj + dj;
                if (!IndexIsValid(ni, nj)) continue;

                // Déjà visité ?
                bool seen = false;
                for (int v = 0; v < tail; v++)
                    if (visitedI[v] == ni && visitedJ[v] == nj) { seen = true; break; }
                if (seen) continue;

                // ── Panneau solaire : source limitée (solarPool) ─────────
                if (grid[ni][nj].up_texture.id == solarpanelTexture.id &&
                    solarPool >= amount) {
                    solarPool -= amount;
                    int idx = head - 1;
                    while (idx >= 0) {
                        int pi = visitedI[idx], pj = visitedJ[idx];
                        for (int k = 0; k < MAX_CONVEYOR; k++) {
                            if (ListeConveyor[k].placed &&
                                ListeConveyor[k].texture.id == piloneTexture.id &&
                                ListeConveyor[k].i == pi && ListeConveyor[k].j == pj) {
                                ListeConveyor[k].load += amount;
                                if (ListeConveyor[k].load > ListeConveyor[k].peak_load) ListeConveyor[k].peak_load = ListeConveyor[k].load;
                            }
                        }
                        idx = parentIdx[idx];
                    }
                    dbg_success++; return 1;
                }

                // ── Steam / Battery : source consommable ──────────────────
                for (int k = 0; k < numSteams; k++) {
                    if (ListeSteam[k].i == ni && ListeSteam[k].j == nj &&
                        ListeSteam[k].final_q >= amount) {
                        ListeSteam[k].final_q -= amount;
                        // Remonter chemin
                        int idx = head - 1;
                        while (idx >= 0) {
                            int pi = visitedI[idx], pj = visitedJ[idx];
                            for (int c = 0; c < MAX_CONVEYOR; c++) {
                                if (ListeConveyor[c].placed &&
                                    ListeConveyor[c].texture.id == piloneTexture.id &&
                                    ListeConveyor[c].i == pi && ListeConveyor[c].j == pj)
                                    ListeConveyor[c].load += amount;
                                    if (ListeConveyor[c].load > ListeConveyor[c].peak_load) ListeConveyor[c].peak_load = ListeConveyor[c].load;
                            }
                            idx = parentIdx[idx];
                        }
                        dbg_success++; return 1;
                    }
                }
                if (FindNearestBattery(ni, nj)) {
                    int idx = head - 1;
                    while (idx >= 0) {
                        int pi = visitedI[idx], pj = visitedJ[idx];
                        for (int c = 0; c < MAX_CONVEYOR; c++) {
                            if (ListeConveyor[c].placed &&
                                ListeConveyor[c].texture.id == piloneTexture.id &&
                                ListeConveyor[c].i == pi && ListeConveyor[c].j == pj)
                                ListeConveyor[c].load += amount;
                                    if (ListeConveyor[c].load > ListeConveyor[c].peak_load) ListeConveyor[c].peak_load = ListeConveyor[c].load;
                        }
                        idx = parentIdx[idx];
                    }
                    dbg_success++; return 1;
                }

                // ── Pylône allumé avec capacité disponible : continuer BFS ─
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
    return 0;  // Pas de source trouvée ou réseau saturé
}

void Update_Conv() {
    // ── Étape 1 : reset les charges des pylônes + recharge solaire ────────
    solarPool = 0;
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (ListeConveyor[k].placed && ListeConveyor[k].texture.id == piloneTexture.id) {
            ListeConveyor[k].load = 0;
            // peak_load conservé jusqu'au prochain F1
            ListeConveyor[k].textureToMove = (Texture2D){0};
            grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture = (Texture2D){0};
        }
    }
    // Compter les panneaux solaires posés
    for (int i = 0; i < COL; i++)
        for (int j = 0; j < ROW; j++)
            if (grid[i][j].up_texture.id == solarpanelTexture.id)
                solarPool += SOLAR_PER_PANEL;

    // ── Étape 2 : allumer les pylônes qui ont une source (HasEnergySource) ─
    // Multi-pass pour propager les chaînes
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

    // ── Étape 3 : convoyeurs normaux ──────────────────────────────────────
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (ListeConveyor[k].placed && ListeConveyor[k].texture.id != piloneTexture.id)
            Convey(&ListeConveyor[k]);
    }
}

// ─── Debug réseau énergétique ─────────────────────────────────────────────────
// Appeler depuis main.c avec la touche F1 pour afficher l'état du réseau.
// Désactiver en production en retirant l'appel, pas le code.
void DebugEnergy(void) {
    printf("\n=== RÉSEAU ÉNERGÉTIQUE ===\n");
    int snap_requests = dbg_requests;
    int snap_success  = dbg_success;
    dbg_requests = 0;
    dbg_success  = 0;
    printf("  RequestEnergy : %d appels, %d succès (depuis dernier F1)\n",
           snap_requests, snap_success);

    int totalSteam = 0;
    for (int i = 0; i < numSteams; i++) {
        printf("  Steam  (%3d,%3d) : %3d énergie stockée\n",
               ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].final_q);
        totalSteam += ListeSteam[i].final_q;
    }
    printf("  Solaire        : %3dA disponibles ce tick (%d panneaux × %dA)\n",
           solarPool, solarPool / SOLAR_PER_PANEL, SOLAR_PER_PANEL);

    int piloneCount = 0;
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (!ListeConveyor[k].placed) continue;
        if (ListeConveyor[k].texture.id != piloneTexture.id) continue;
        bool allume = grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture.id != 0;
        printf("  Pylône (%3d,%3d) : now=%d/5A  peak=%d/5A  [%s]\n",
               ListeConveyor[k].i, ListeConveyor[k].j,
               ListeConveyor[k].load, ListeConveyor[k].peak_load,
               allume ? "ALLUMÉ" : "éteint");
        ListeConveyor[k].peak_load = 0;
        piloneCount++;
    }
    if (piloneCount == 0) printf("  (aucun pylône posé)\n");

    printf("  Foreuses  : %d (tick toutes les 5s)\n", numForeuses);
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

    if (piloneCount > 0 && totalSteam > 0 && snap_requests == 0)
        printf("  [!] Steam + pylônes OK mais 0 appels RequestEnergy = machines sans timer actif\n");

    printf("=========================\n");
}

void UpdateBattery() {
    for (int k = 0; k < MAX_BATTERY; k++) {
        if (ListeBattery[k].placed && ListeBattery[k].q <= 100) {
            if (RequestEnergy(ListeBattery[k].i, ListeBattery[k].j, 1))
                ListeBattery[k].q++;
        }
    }
}
void Convey(Conveyor *conv) {
    
    int srcI = conv->i - conv->dir[0];  // Calcul de la case source
    int srcJ = conv->j - conv->dir[1];  // Calcul de la case source
    int destI = conv->i + conv->dir[0]; // Calcul de la case destination
    int destJ = conv->j + conv->dir[1]; // Calcul de la case destination

    if (!IndexIsValid(srcI, srcJ) || !IndexIsValid(destI, destJ)) {
        return; // Eviter l'accès aux indices invalides
    }

    if (grid[srcI][srcJ].moveable &&
        grid[srcI][srcJ].up_texture.id != 0 && 
        grid[srcI][srcJ].up_texture.id != conv->texture.id) {
        if((grid[srcI][srcJ].isSolid && conv->texture.id==conveyorTexture.id) || (!grid[srcI][srcJ].isSolid && conv->texture.id==pipeTexture.id)){
            conv->textureToMove = grid[srcI][srcJ].up_texture;
            grid[srcI][srcJ].placed = false;
            grid[srcI][srcJ].up_texture = (Texture2D){ 0 }; // Effacer la case source
            grid[conv->i][conv->j].move_texture=conv->textureToMove;
        }
    }
    else if(grid[srcI][srcJ].up_texture.id == drillTexture.id) {
            for (int k = 0; k < numForeuses; k++) {
                if (ListeForeuse[k].i == srcI && ListeForeuse[k].j == srcJ) {
                    if (ListeForeuse[k].q>0){
                        Texture2D under_texture = grid[ListeForeuse[k].i][ListeForeuse[k].j].texture;
                        Texture2D mined_texture =(Texture2D){0};
                        if(conv->texture.id == conveyorTexture.id && grid[srcI][srcJ].isSolid){ //solid
                            if (under_texture.id == copperVeinTexture.id) {
                                mined_texture = copperTexture;
                            }
                            else if (under_texture.id == ironVeinTexture.id) {
                                mined_texture = ironTexture;
                            }
                            else if (under_texture.id == coalVeinTexture.id) {
                                mined_texture = coalTexture;
                            } 
                        }
                        else if(conv->texture.id == pipeTexture.id ){ //liquide
                            if (under_texture.id == waterVeinTexture.id) {
                                mined_texture = waterVeinTexture;
                            }
                            if (under_texture.id == oilVeinTexture.id) {
                                mined_texture = oilVeinTexture;
                            }
                            }
                        conv->textureToMove = mined_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeForeuse[k].q--;
                    }
                    
                }
            }
    }
    else if(grid[srcI][srcJ].up_texture.id == furnaceTexture.id ) {
            for (int k = 0; k < numForeuses; k++) {
                if (ListeFurnace[k].i == srcI && ListeFurnace[k].j == srcJ) {
                    if (ListeFurnace[k].final_q>0){
                        int crafted_textureId = ListeFurnace[k].final_id;
                        Texture2D crafted_texture =(Texture2D){0};
                        if (crafted_textureId == copperLingotTexture.id) {
                            crafted_texture = copperLingotTexture;
                        }
                        else if (crafted_textureId == ironLingotTexture.id) {
                            crafted_texture = ironLingotTexture;
                        }
                       
                        conv->textureToMove = crafted_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeFurnace[k].final_q--;
                    }
                    
                }
            }
    }

else if(grid[srcI][srcJ].up_texture.id == pressTexture.id ) {
            for (int k = 0; k < numHydraulics; k++) {
                if (ListeHydraulic[k].i == srcI && ListeHydraulic[k].j == srcJ) {
                    if (ListeHydraulic[k].final_q>0){
                        int crafted_textureId = ListeHydraulic[k].final_id;
                        Texture2D crafted_texture =(Texture2D){0};

                        if (crafted_textureId == ironPlateTexture.id) {
                            crafted_texture = ironPlateTexture;
                        }
                        
                        else if (crafted_textureId == copperPlateTexture.id) {
                            crafted_texture = copperPlateTexture;
                        }
                        else if (crafted_textureId == gearTexture.id) {
                            crafted_texture = gearTexture;
                        }
                       
                        conv->textureToMove = crafted_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeHydraulic[k].final_q--;
                    }
                    
                }
            }
    }
    
    else if(grid[srcI][srcJ].up_texture.id == stretchTexture.id ) {
            for (int k = 0; k < numEttireuses; k++) {
                if (ListeEttireuse[k].i == srcI && ListeEttireuse[k].j == srcJ) {
                    if (ListeEttireuse[k].final_q>0){
                        int crafted_textureId = ListeEttireuse[k].final_id;
                        Texture2D crafted_texture =(Texture2D){0};

                        if (crafted_textureId == copperRodTexture.id) {
                            crafted_texture = copperRodTexture;
                        }
                        else if (crafted_textureId == ironRodTexture.id) {
                            crafted_texture = ironRodTexture;
                        }
                        else if (crafted_textureId == copperCableTexture.id) {
                            crafted_texture = copperCableTexture;
                        }
                       
                        conv->textureToMove = crafted_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeEttireuse[k].final_q--;
                    }
                    
                }
            }
    }

    if (grid[srcI][srcJ].move_texture.id != 0 && grid[conv->i][conv->j].move_texture.id==0 && (grid[srcI][srcJ].up_texture.id ==conveyorTexture.id || grid[srcI][srcJ].up_texture.id ==pipeTexture.id || grid[srcI][srcJ].up_texture.id ==piloneTexture.id) ) {
        conv->textureToMove=grid[srcI][srcJ].move_texture;
        grid[srcI][srcJ].move_texture = (Texture2D){ 0 }; // Réinitialiser move_texture
        grid[conv->i][conv->j].move_texture=conv->textureToMove;
        }
    if (grid[destI][destJ].up_texture.id !=conv->texture.id && !grid[destI][destJ].placed && grid[conv->i][conv->j].move_texture.id!=0) {

    }
    else if (grid[destI][destJ].up_texture.id !=conv->texture.id && !grid[destI][destJ].placed && grid[srcI][srcJ].moveable){
         conv->textureToMove=grid[srcI][srcJ].move_texture;
         grid[conv->i][conv->j].move_texture=conv->textureToMove;
    }
    else if (grid[destI][destJ].up_texture.id == chestTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        AddInInvent(1, conv->textureToMove);
        grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
        conv->textureToMove = (Texture2D){ 0 };
    }
    else if (grid[destI][destJ].up_texture.id == furnaceTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numFurnaces; k++) {
                if (ListeFurnace[k].i == destI && ListeFurnace[k].j == destJ) {
                        if (ListeFurnace[k].material_id == conv->textureToMove.id && conv->textureToMove.id != coalTexture.id ) {
                            ListeFurnace[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeFurnace[k].material_q ==0 && conv->textureToMove.id != coalTexture.id ) {
                            ListeFurnace[k].material_id = conv->textureToMove.id;
                            ListeFurnace[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeFurnace[k].energy_id == conv->textureToMove.id && conv->textureToMove.id == coalTexture.id ) {
                            ListeFurnace[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                        else if (ListeFurnace[k].energy_q == 0 && conv->textureToMove.id == coalTexture.id ) {
                            ListeFurnace[k].energy_id= conv->textureToMove.id;
                            ListeFurnace[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                }
            }
    }
    else if (grid[destI][destJ].up_texture.id == pressTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numHydraulics; k++) {
                if (ListeHydraulic[k].i == destI && ListeHydraulic[k].j == destJ) {
                        if (ListeHydraulic[k].material_id == conv->textureToMove.id && conv->textureToMove.id != coalTexture.id ) {
                            ListeHydraulic[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeHydraulic[k].material_q ==0 && conv->textureToMove.id != coalTexture.id ) {
                            ListeHydraulic[k].material_id = conv->textureToMove.id;
                            ListeHydraulic[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }      
                }
            }
    }
    else if (grid[destI][destJ].up_texture.id == stretchTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numEttireuses; k++) {
                if (ListeEttireuse[k].i == destI && ListeEttireuse[k].j == destJ) {
                    
                        if (ListeEttireuse[k].material_id == conv->textureToMove.id && conv->textureToMove.id != 0 ) {
                            ListeEttireuse[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeEttireuse[k].material_q ==0 && conv->textureToMove.id != 0 ) {
                            ListeEttireuse[k].material_id = conv->textureToMove.id;
                            ListeEttireuse[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }      
                }
            }
    }
    else if (grid[destI][destJ].up_texture.id == steamcentralTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numSteams; k++) {
                if (ListeSteam[k].i == destI && ListeSteam[k].j == destJ) {
                        if (ListeSteam[k].material_id == conv->textureToMove.id && conv->textureToMove.id != coalTexture.id ) {
                            ListeSteam[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeSteam[k].material_q ==0 && conv->textureToMove.id != coalTexture.id ) {
                            ListeSteam[k].material_id = conv->textureToMove.id;
                            ListeSteam[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeSteam[k].energy_id == conv->textureToMove.id && (conv->textureToMove.id == coalTexture.id  || conv->textureToMove.id == oilVeinTexture.id)) {
                            ListeSteam[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                        else if (ListeSteam[k].energy_q == 0 && (conv->textureToMove.id == coalTexture.id  || conv->textureToMove.id == oilVeinTexture.id)) {
                            ListeSteam[k].energy_id= conv->textureToMove.id;
                            ListeSteam[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                }
            }
    }
}

void Update_Foreuse() {
    float currentTime = GetTime();
    if (currentTime - lastForeuseTime >= 3.0f) {
        for (int i = 0; i < numForeuses; i++) {
            if (ListeForeuse[i].placed && IndexIsValid(ListeForeuse[i].i, ListeForeuse[i].j) &&
                RequestEnergy(ListeForeuse[i].i, ListeForeuse[i].j, 1)) {
                Texture2D texture = grid[ListeForeuse[i].i][ListeForeuse[i].j].texture;
                if (ListeForeuse[i].q < 100) {
                    if      (texture.id == copperVeinTexture.id)  ListeForeuse[i].q += 2;
                    else if (texture.id == ironVeinTexture.id)    ListeForeuse[i].q += 2;
                    else if (texture.id == coalVeinTexture.id)    ListeForeuse[i].q += 3; // charbon plus abondant
                    else if (texture.id == waterVeinTexture.id)   ListeForeuse[i].q += 2;
                    else if (texture.id == oilVeinTexture.id)     ListeForeuse[i].q += 2;
                    else if (texture.id == rockVeinTexture.id)    ListeForeuse[i].q += 2;
                    else if (texture.id == sandVeinTexture.id)    ListeForeuse[i].q += 3; // sable très abondant
                }
            }
        }
        lastForeuseTime = currentTime;
    }
}

bool isForeuse(int posX, int posY) {
    for (int i = 0; i < numForeuses; i++) {
        if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) {
            return true;
        }
    }
    return false;
}

bool isFurnace(int posX, int posY)   { return isMachine(ListeFurnace,   numFurnaces,   posX, posY); }
bool isHydraulic(int posX, int posY) { return isMachine(ListeHydraulic,  numHydraulics, posX, posY); }
bool isEttireuse(int posX, int posY) { return isMachine(ListeEttireuse,  numEttireuses, posX, posY); }

bool isSteam(int posX, int posY) {
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == posX && ListeSteam[i].j == posY) {
            return true;
        }
    }
    return false;
}

bool isOil(int posX, int posY) {
    for (int i = 0; i < numOils; i++) {
        if (ListeOil[i].i == posX && ListeOil[i].j == posY) {
            return true;
        }
    }
    return false;
}

void RemoveForeuse(int posX, int posY) {
    for (int i = 0; i < numForeuses; i++) {
        if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) {
            for (int j = i; j < numForeuses - 1; j++) {
                ListeForeuse[j] = ListeForeuse[j + 1];
            }
            numForeuses--;
            grid[posX][posY].move_texture=(Texture2D){0};
            return;
        }
    }
}

void RemoveFurnace(int posX, int posY) {
    Remove_Machine(ListeFurnace, &numFurnaces, posX, posY);
}

void RemoveHydraulic(int posX, int posY) {
    Remove_Machine(ListeHydraulic, &numHydraulics, posX, posY);
}

void RemoveEttireuse(int posX, int posY) {
    Remove_Machine(ListeEttireuse, &numEttireuses, posX, posY);
}

void RemoveSteam(int posX, int posY) {
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == posX && ListeSteam[i].j == posY) {
            for (int j = i; j < numSteams - 1; j++) {
                ListeSteam[j] = ListeSteam[j + 1];
            }
            numSteams--;
           grid[posX][posY].move_texture=(Texture2D){0};

            return;
        }
    }
}

void RemoveOil(int posX, int posY) {
    for (int i = 0; i < numOils; i++) {
        if (ListeOil[i].i == posX && ListeOil[i].j == posY) {
            for (int j = i; j < numOils - 1; j++) {
                ListeOil[j] = ListeOil[j + 1];
            }
            numOils--;
            grid[posX][posY].moveable = true;
            grid[posX][posY].move_texture=(Texture2D){0};
            return;

        }
    }
}

void RemoveConveyor(int posX, int posY){
    for (int i = 0; i < MAX_CONVEYOR; i++) {
        if (ListeConveyor[i].i == posX && ListeConveyor[i].j == posY) {
            for (int j = i; j < MAX_CONVEYOR - 1; j++) {
                ListeConveyor[j] = ListeConveyor[j + 1];
            }
            grid[posX][posY].moveable = true;
            grid[posX][posY].move_texture=(Texture2D){0};
            return;
            
        }
    }
}

void RemoveBattery(int posX, int posY){
    for (int i = 0; i < MAX_BATTERY; i++) {
        if (ListeBattery[i].i == posX && ListeBattery[i].j == posY) {
            for (int j = i; j < MAX_BATTERY - 1; j++) {
                ListeBattery[j] = ListeBattery[j + 1];
            }
            grid[posX][posY].moveable = true;
           grid[posX][posY].move_texture=(Texture2D){0};

            return;
        }
    }
}

void Update_Furnace() {
    Update_Processor(ListeFurnace, numFurnaces,
                     &lastFurnaceTime, 10.0f, true,
                     furnaceEffect,
                     FurnaceRecipes, FurnaceRecipeCount);
}

void Update_Hydraulic() {
    Update_Processor(ListeHydraulic, numHydraulics,
                     &lastHydraulicTime, 8.0f, true,
                     pressEffect,
                     HydraulicRecipes, HydraulicRecipeCount);
}

void Update_Ettireuse() {
    Update_Processor(ListeEttireuse, numEttireuses,
                     &lastEttireuseTime, 8.0f, true,
                     stretchEffect,
                     EttireuseRecipes, EttireuseRecipeCount);
}

void Update_Steam() {
    float currentTime = GetTime();
    if (currentTime - lastSteamTime >= 10.0f) {
        for (int i = 0; i < numSteams; i++) {
            if (ListeSteam[i].placed && IndexIsValid(ListeSteam[i].i, ListeSteam[i].j)) {
                if (ListeSteam[i].energy_q > 0 && ListeSteam[i].material_q > 0) {
                    grid[ListeSteam[i].i][ListeSteam[i].j].move_texture=steamEffect;
                    if (ListeSteam[i].material_id == waterVeinTexture.id) {
                        if (ListeSteam[i].final_q < 100) {  // cap 100 (était 50)
                            if (ListeSteam[i].energy_id == oilVeinTexture.id) {
                                ListeSteam[i].energy_q--;
                                ListeSteam[i].material_q--;
                                ListeSteam[i].final_q += 12;  // était 6
                            }
                            else if (ListeSteam[i].energy_id == coalTexture.id) {
                                ListeSteam[i].energy_q--;
                                ListeSteam[i].material_q--;
                                ListeSteam[i].final_q += 8;   // était 4
                            }
                        }
                        if (ListeSteam[i].energy_q == 0) {
                            ListeSteam[i].energy_id = 0;
                        }
                        if (ListeSteam[i].material_q == 0) {
                            ListeSteam[i].material_id = 0;
                        }
                    }
                } else {
                     grid[ListeSteam[i].i][ListeSteam[i].j].move_texture=(Texture2D){0};
                }
            }
        }
        lastSteamTime = currentTime;
    }
}

void Update_Oil() {
    float currentTime = GetTime();
    if (currentTime - lastOilTime >= 20.0f) {
        for (int i = 0; i < numOils; i++) {
            if (ListeOil[i].placed && IndexIsValid(ListeOil[i].i, ListeOil[i].j)) {
                if (ListeOil[i].energy_q > 0 && ListeOil[i].material_q > 0) {
                    if (ListeOil[i].material_id == waterVeinTexture.id) {
                        if (ListeOil[i].final_q < 50) {
                            ListeOil[i].energy_q--;
                            ListeOil[i].material_q--;
                            ListeOil[i].final_q += 8;
                        }
                        if (ListeOil[i].energy_q   == 0) ListeOil[i].energy_id   = 0;
                        if (ListeOil[i].material_q == 0) ListeOil[i].material_id = 0;
                    }
                }
            }
        }
        lastOilTime = currentTime;
    }
}

int AddInInvent(int q, Texture2D texture) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (texture.id == inventory[i].texture.id) {
            inventory[i].quantity += q;
            return 0;
        }
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i].quantity == 0) {
            inventory[i].quantity = q;
            inventory[i].texture = texture;

            const char* name = FindName(texture);
            if (name != NULL) {
                strncpy(inventory[i].name, name, sizeof(inventory[i].name) - 1);
                inventory[i].name[sizeof(inventory[i].name) - 1] = '\0'; // Ensure null-termination
            } else {
                inventory[i].name[0] = '\0'; // Handle missing name
            }
            return 0; // Success
        }
    }
    return -1;
}

const char* FindName(Texture2D textureRef)
{
    // Base, Struct, Meca ont MaxBaseCraft items
    for (int k = 0; k < MaxBaseCraft; k++) {
        if (BaseCraftInvent[k].texture.id  == textureRef.id) return BaseCraftInvent[k].name;
        if (StructCraftInvent[k].texture.id == textureRef.id) return StructCraftInvent[k].name;
        if (MecaCraftInvent[k].texture.id  == textureRef.id) return MecaCraftInvent[k].name;
    }
    // ProdCraft peut avoir plus d'items que MaxBaseCraft — on cherche jusqu'au nom vide
    for (int k = 0; k < MaxBaseCraft * 2; k++) {
        if (ProdCraftInvent[k].name[0] == '\0') break;
        if (ProdCraftInvent[k].texture.id == textureRef.id) return ProdCraftInvent[k].name;
    }
    return " ";
}

// ─── Helper générique : interaction joueur ↔ machine ────────────────────────
// energy_tex    : texture acceptée comme énergie (0 = aucune)
// mat_tex[]     : textures acceptées comme matériau (tableau terminé par id==0)
// Collecte automatiquement final_q dans l'inventaire.
static void interact_machine(Machine *m,
                              unsigned int energy_tex_id,
                              unsigned int mat_tex_ids[]) {
    Item *sel = &inventory[selectedItem];
    unsigned int sel_id = sel->texture.id;

    if (sel->quantity <= 0) {
        // Pas d'item sélectionné : on tente de récupérer la production
        if (m->final_q > 0 && m->final_id != 0) {
            Texture2D out = IdxToTex(TexToIdx((Texture2D){.id = m->final_id}));
            if (out.id == 0) out.id = m->final_id; // fallback si non indexé
            AddInInvent(m->final_q, out);
            m->final_q = 0;
            m->final_id = 0;
        }
        return;
    }

    // Énergie
    if (energy_tex_id != 0 && sel_id == energy_tex_id) {
        sel->quantity--;
        m->energy_q++;
        m->energy_id = energy_tex_id;
        return;
    }

    // Matériaux acceptés
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

    // Récupération production (clic avec mauvais item en main)
    if (m->final_q > 0 && m->final_id != 0) {
        Texture2D out = IdxToTex(TexToIdx((Texture2D){.id = m->final_id}));
        if (out.id == 0) out.id = m->final_id;
        AddInInvent(m->final_q, out);
        m->final_q = 0;
        m->final_id = 0;
    }
}

// ─── Matériaux acceptés par type de machine ──────────────────────────────────
// Tableaux terminés par 0 — ajoutez ici pour étendre les recettes
static unsigned int FurnaceMats[]   = { 0, 0, 0, 0 }; // rempli dans interraction
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
            if      (tex.id == copperVeinTexture.id) { AddInInvent(ListeForeuse[i].q, copperTexture); }
            else if (tex.id == ironVeinTexture.id)   { AddInInvent(ListeForeuse[i].q, ironTexture); }
            else if (tex.id == coalVeinTexture.id)   { AddInInvent(ListeForeuse[i].q, coalTexture); }
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

int FindNearestSteam(int x,int y){
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == x && ListeSteam[i].j == y &&ListeSteam[i].final_q>0) {
            ListeSteam[i].final_q--;
            return 1;
        }
    }
    return 0;
}