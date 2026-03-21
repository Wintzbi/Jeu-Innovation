#include "input.h"
#include "energy.h"
#include "production.h"
#include "conveyor.h"
#include "cell.h"
#include "inventory.h"
#include "camera.h"
#include "texture_registry.h"
#include <stdio.h>
#include <string.h>

// ─── Variables globales ───────────────────────────────────────────────────────
int MinPlaceableID = 23;
float rotation     = 0.0f;
bool infoMode      = false;
int conveyor_dir   = 0;
int option         = 0;

Conveyor ListeConveyor[MAX_CONVEYOR];
Foreuse  ListeForeuse[MAX_FOREUSE];
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
int numSteams = 0;
float lastSteamTime;

Oil ListeOil[MAX_OIL];
int numOils = 0;
float lastOilTime;

Battery ListeBattery[MAX_BATTERY];

int directions[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

// ─── mouseDefault ─────────────────────────────────────────────────────────────
void mouseDefault(void) {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (inventory[selectedItem].texture.id != 0 &&
        inventory[selectedItem].texture.id >= MinPlaceableID) {
        Rectangle destRect = { posX * cellSize + cellSize / 2.0f,
                               posY * cellSize + cellSize / 2.0f, cellSize, cellSize };
        Rectangle srcRect  = { 0, 0,
                               inventory[selectedItem].texture.width,
                               inventory[selectedItem].texture.height };
        Vector2 origin = { cellSize / 2.0f, cellSize / 2.0f };
        unsigned int id = inventory[selectedItem].texture.id;
        float rot = (id == conveyorTexture.id || id == pipeTexture.id || id == piloneTexture.id)
                  ? rotation : 0.0f;
        DrawTexturePro(inventory[selectedItem].texture, srcRect, destRect, origin,
                       rot, Fade(WHITE, 0.35f));
    } else {
        DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(RED, 0.35f));
    }
}

// ─── InitInventoryKeyBiding ───────────────────────────────────────────────────
void InitInventoryKeyBiding(void) {
    if (IsKeyPressed(KEY_ONE))   selectedItem = 0;
    if (IsKeyPressed(KEY_TWO))   selectedItem = 1;
    if (IsKeyPressed(KEY_THREE)) selectedItem = 2;
    if (IsKeyPressed(KEY_FOUR))  selectedItem = 3;
    if (IsKeyPressed(KEY_FIVE))  selectedItem = 4;
    if (IsKeyPressed(KEY_SIX))   selectedItem = 5;
    if (IsKeyPressed(KEY_SEVEN)) selectedItem = 6;
    if (IsKeyPressed(KEY_EIGHT)) selectedItem = 7;
    if (IsKeyPressed(KEY_NINE))  selectedItem = 8;
    if (IsKeyPressed(KEY_ZERO))  selectedItem = 9;
}

// ─── rightClic ────────────────────────────────────────────────────────────────
void rightClic(void) {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (!IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) return;

    interraction(posX, posY);

    if (inventory[selectedItem].quantity > 0 &&
        inventory[selectedItem].texture.id >= MinPlaceableID) {
        if (IndexIsValid(posX, posY) && !grid[posX][posY].placed) {
            grid[posX][posY].placed     = true;
            grid[posX][posY].up_texture = inventory[selectedItem].texture;
            ActionWithName(inventory[selectedItem].name, posX, posY, option);
            inventory[selectedItem].quantity--;
            if (inventory[selectedItem].texture.id == solarpanelTexture.id)
                AddSolarPanel(posX, posY);
            if (inventory[selectedItem].quantity == 0)
                inventory[selectedItem].texture = (Texture2D){0};
        }
    }
}

// ─── leftClic ────────────────────────────────────────────────────────────────
void leftClic(void) {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    if (!IndexIsValid(posX, posY)) return;
    if (!grid[posX][posY].placed || !grid[posX][posY].pickable) return;
    if (grid[posX][posY].up_texture.id == 0) return;

    Texture2D broken = grid[posX][posY].up_texture;
    bool found = false;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i].texture.id == broken.id) {
            inventory[i].quantity++;
            found = true;
            break;
        }
    }
    if (!found) {
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            if (inventory[i].quantity == 0) {
                inventory[i].texture  = broken;
                inventory[i].quantity = 1;
                break;
            }
        }
    }

    grid[posX][posY].up_texture = (Texture2D){0};
    grid[posX][posY].placed     = false;
    grid[posX][posY].dir[0]     = 0;
    grid[posX][posY].dir[1]     = 0;
    pickedObject++;

    if (broken.id == solarpanelTexture.id) RemoveSolarPanel(posX, posY);

    // Foreuses
    for (int f = 0; f < numForeuses; f++) {
        if (ListeForeuse[f].i == posX && ListeForeuse[f].j == posY && ListeForeuse[f].placed) {
            ListeForeuse[f].placed = false;
            RemoveForeuse(posX, posY);
            grid[posX][posY].moveable = true;
            break;
        }
    }
    // Batteries
    for (int f = 0; f < MAX_BATTERY; f++) {
        if (ListeBattery[f].i == posX && ListeBattery[f].j == posY && ListeBattery[f].placed) {
            ListeBattery[f].placed = false;
            RemoveBattery(posX, posY);
            break;
        }
    }
    // Convoyeurs
    for (int f = 0; f < MAX_CONVEYOR; f++) {
        if (ListeConveyor[f].i == posX && ListeConveyor[f].j == posY && ListeConveyor[f].placed) {
            ListeConveyor[f].placed = false;
            grid[posX][posY].dir[0] = 0;
            grid[posX][posY].dir[1] = 0;
            grid[posX][posY].move_texture = (Texture2D){0};
            RemoveConveyor(posX, posY);
            break;
        }
    }
    // Machines processeurs
    if (isMachine(ListeFurnace,   numFurnaces,   posX, posY)) { RemoveFurnace(posX, posY);   grid[posX][posY].moveable = true; }
    if (isMachine(ListeHydraulic, numHydraulics, posX, posY)) { RemoveHydraulic(posX, posY); grid[posX][posY].moveable = true; }
    if (isMachine(ListeEttireuse, numEttireuses, posX, posY)) { RemoveEttireuse(posX, posY); grid[posX][posY].moveable = true; }
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

// ─── UpdateDir ────────────────────────────────────────────────────────────────
void UpdateDir(void) {
    conveyor_dir = (conveyor_dir + 1) % 4;
    rotation = (float)(conveyor_dir * 90);
}

// ─── ActionWithName ───────────────────────────────────────────────────────────
void ActionWithName(char ObjectName[20], int i, int j, int opt) {
    (void)opt;
    unsigned int id = inventory[selectedItem].texture.id;

    if (id == conveyorTexture.id || id == pipeTexture.id || id == piloneTexture.id) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (ListeConveyor[k].placed) continue;
            grid[i][j].dir[0] = directions[conveyor_dir][0];
            grid[i][j].dir[1] = directions[conveyor_dir][1];
            Texture2D tex = (id == conveyorTexture.id) ? conveyorTexture
                          : (id == pipeTexture.id)     ? pipeTexture : piloneTexture;
            ListeConveyor[k] = (Conveyor){
                .i = i, .j = j, .texture = tex,
                .max_load = (id == piloneTexture.id) ? 5 : 0,
                .capacity = 2,
                .dir = {directions[conveyor_dir][0], directions[conveyor_dir][1]},
                .placed = true, .textureToMove = (Texture2D){0}
            };
            if (id == pipeTexture.id) grid[i][j].moveable = false;
            break;
        }
    } else if (id == batteryTexture.id) {
        for (int k = 0; k < MAX_BATTERY; k++) {
            if (!ListeBattery[k].placed) {
                ListeBattery[k] = (Battery){.i=i,.j=j,.texture=batteryTexture,.q=0,.placed=true};
                break;
            }
        }
    } else if (id == drillTexture.id) {
        if (numForeuses < MAX_FOREUSE) {
            ListeForeuse[numForeuses++] = (Foreuse){.i=i,.j=j,.q=0,.placed=true};
            grid[i][j].moveable = false;
        }
    } else if (id == furnaceTexture.id) {
        if (numFurnaces < MAX_FURNACE) {
            ListeFurnace[numFurnaces++] = (Machine){.i=i,.j=j,.placed=true};
            grid[i][j].moveable = false;
        }
    } else if (id == pressTexture.id) {
        if (numHydraulics < MAX_HYDRAULIC) {
            ListeHydraulic[numHydraulics++] = (Machine){.i=i,.j=j,.placed=true};
            grid[i][j].moveable = false;
        }
    } else if (id == stretchTexture.id) {
        if (numEttireuses < MAX_ETTIREUSE) {
            ListeEttireuse[numEttireuses++] = (Machine){.i=i,.j=j,.placed=true};
            grid[i][j].moveable = false;
        }
    } else if (id == steamcentralTexture.id) {
        if (numSteams < MAX_STEAM) {
            ListeSteam[numSteams++] = (Steam){.i=i,.j=j,.placed=true};
            grid[i][j].moveable = false;
        }
    } else if (strcmp(ObjectName, "Centrale Pétrole") == 0) {
        if (numOils < MAX_OIL) {
            ListeOil[numOils++] = (Oil){.i=i,.j=j,.placed=true};
            grid[i][j].moveable = false;
        }
    }
}

// ─── AddInInvent / FindName ───────────────────────────────────────────────────
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
            inventory[i].texture  = texture;
            const char *name = FindName(texture);
            if (name) {
                strncpy(inventory[i].name, name, sizeof(inventory[i].name) - 1);
                inventory[i].name[sizeof(inventory[i].name) - 1] = '\0';
            } else {
                inventory[i].name[0] = '\0';
            }
            return 0;
        }
    }
    return -1;
}

const char* FindName(Texture2D textureRef) {
    for (int k = 0; k < MaxBaseCraft; k++) {
        if (BaseCraftInvent[k].texture.id   == textureRef.id) return BaseCraftInvent[k].name;
        if (StructCraftInvent[k].texture.id == textureRef.id) return StructCraftInvent[k].name;
        if (MecaCraftInvent[k].texture.id   == textureRef.id) return MecaCraftInvent[k].name;
    }
    for (int k = 0; k < MaxBaseCraft * 2; k++) {
        if (ProdCraftInvent[k].name[0] == '\0') break;
        if (ProdCraftInvent[k].texture.id == textureRef.id) return ProdCraftInvent[k].name;
    }
    return " ";
}