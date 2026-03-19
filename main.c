#include "global.h"
#include "texture_registry.h"

GameScreen currentScreen = MENU;
bool isInventoryScreenOpen = false;
bool isOptionScreenOpen = false;
bool isCraftScreenOpen=false;
bool isMapScreenOpen=false;
bool isGameInitialized = false;
void CurrentScreenFix(){
    if(currentScreen == GAME)
    {
        isInventoryScreenOpen = false;
        isOptionScreenOpen = false;
        isCraftScreenOpen=false;
        isMapScreenOpen=false;
        }
}

// ─── Helpers save/load ───────────────────────────────────────────────────────
// Écrit/lit un int dans le fichier (texture index ou valeur simple)
#define WINT(v)  { int _v=(v); fwrite(&_v, sizeof(int), 1, file); }
#define RINT(v)  fread(&(v), sizeof(int), 1, file)

// Convertit un OpenGL texture.id en index logique du registre
// (pour les champs material_id / energy_id / final_id qui stockent des .id GPU)
static int RawIdToIdx(unsigned int rawId) {
    if (rawId == 0) return TEX_NONE;
    Texture2D t = { .id = rawId };
    return TexToIdx(t);
}

// ─── save() ──────────────────────────────────────────────────────────────────
int save() {
    FILE *file = fopen("save.dat", "wb");
    if (!file) { perror("save"); return 1; }

    // Version du format — permet de détecter les saves incompatibles
    int version = 5;
    WINT(version);

    // Grille : chaque cellule sérialise ses 3 textures en indices logiques
    int gridSize = ROW * COL;
    WINT(gridSize);
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            Cell *c = &grid[i][j];
            WINT(c->i);  WINT(c->j);
            WINT((int)c->placed);  WINT((int)c->pickable);
            WINT((int)c->moveable); WINT((int)c->isSolid);
            WINT(c->dir[0]);  WINT(c->dir[1]);
            WINT(TexToIdx(c->texture));
            WINT(TexToIdx(c->up_texture));
            WINT(TexToIdx(c->move_texture));
        }
    }

    // Inventaire : Item contient une Texture2D mais pas de handle critique
    // (les items sont recréés par nom au craft — on sauvegarde quand même l'index)
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fwrite(inventory[i].name, sizeof(char), 20, file);
        WINT(TexToIdx(inventory[i].texture));
        WINT(inventory[i].quantity);
    }

    // Foreuses : pas de texture de production, juste position + quantité
    WINT(numForeuses);
    for (int i = 0; i < numForeuses; i++) {
        WINT(ListeForeuse[i].i);  WINT(ListeForeuse[i].j);
        WINT((int)ListeForeuse[i].placed);
        WINT(ListeForeuse[i].q);
    }

    // Machines processeurs (Furnace, Hydraulic, Ettireuse)
    // material_id / energy_id / final_id sont des .id OpenGL → convertis en index
    #define SAVE_MACHINES(list, count) do { \
        WINT(count); \
        for (int i = 0; i < (count); i++) { \
            WINT((list)[i].i);  WINT((list)[i].j); \
            WINT((int)(list)[i].placed); \
            WINT((list)[i].energy_q);  WINT(RawIdToIdx((list)[i].energy_id)); \
            WINT((list)[i].material_q);WINT(RawIdToIdx((list)[i].material_id)); \
            WINT((list)[i].final_q);   WINT(RawIdToIdx((list)[i].final_id)); \
        } \
    } while(0)

    SAVE_MACHINES(ListeFurnace,   numFurnaces);
    SAVE_MACHINES(ListeHydraulic, numHydraulics);
    SAVE_MACHINES(ListeEttireuse, numEttireuses);
    #undef SAVE_MACHINES

    // Générateurs (Steam, Oil) — même logique, pas de final_id
    #define SAVE_GENERATORS(list, count) do { \
        WINT(count); \
        for (int i = 0; i < (count); i++) { \
            WINT((list)[i].i);  WINT((list)[i].j); \
            WINT((int)(list)[i].placed); \
            WINT((list)[i].energy_q);  WINT(RawIdToIdx((list)[i].energy_id)); \
            WINT((list)[i].material_q);WINT(RawIdToIdx((list)[i].material_id)); \
            WINT((list)[i].final_q); \
        } \
    } while(0)

    SAVE_GENERATORS(ListeSteam, numSteams);
    SAVE_GENERATORS(ListeOil,   numOils);
    #undef SAVE_GENERATORS

    // Convoyeurs
    for (int i = 0; i < MAX_CONVEYOR; i++) {
        WINT(ListeConveyor[i].i);  WINT(ListeConveyor[i].j);
        WINT(ListeConveyor[i].dir[0]);  WINT(ListeConveyor[i].dir[1]);
        WINT((int)ListeConveyor[i].placed);
        WINT((int)ListeConveyor[i].power);
        WINT(ListeConveyor[i].max_load);
        WINT(ListeConveyor[i].amount);
        WINT(ListeConveyor[i].capacity);
        WINT(TexToIdx(ListeConveyor[i].texture));
        WINT(TexToIdx(ListeConveyor[i].textureToMove));
    }

    // Batteries
    for (int i = 0; i < MAX_BATTERY; i++) {
        WINT(ListeBattery[i].i);  WINT(ListeBattery[i].j);
        WINT(ListeBattery[i].q);
        WINT((int)ListeBattery[i].placed);
    }

    // Stats
    WINT(days);

    fclose(file);
    return 0;
}

// ─── load() ──────────────────────────────────────────────────────────────────
int load() {
    FILE *file = fopen("save.dat", "rb");
    if (!file) { perror("load"); return 1; }

    int version;
    RINT(version);
    if (version != 4 && version != 5) {
        printf("Save incompatible (version %d), ignoré\n", version);
        fclose(file);
        return 1;
    }
    bool has_max_load = (version >= 4);
    bool has_amount   = (version >= 5);

    int gridSize;
    RINT(gridSize);
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            Cell *c = &grid[i][j];
            int ti, tup, tmove, placed, pickable, moveable, isSolid;
            RINT(c->i);  RINT(c->j);
            RINT(placed);   c->placed   = (bool)placed;
            RINT(pickable); c->pickable = (bool)pickable;
            RINT(moveable); c->moveable = (bool)moveable;
            RINT(isSolid);  c->isSolid  = (bool)isSolid;
            RINT(c->dir[0]);  RINT(c->dir[1]);
            RINT(ti);    c->texture      = IdxToTex(ti);
            RINT(tup);   c->up_texture   = IdxToTex(tup);
            RINT(tmove); c->move_texture = IdxToTex(tmove);
        }
    }

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int tidx;
        fread(inventory[i].name, sizeof(char), 20, file);
        RINT(tidx); inventory[i].texture = IdxToTex(tidx);
        RINT(inventory[i].quantity);
    }

    RINT(numForeuses);
    for (int i = 0; i < numForeuses; i++) {
        RINT(ListeForeuse[i].i);  RINT(ListeForeuse[i].j);
        int fp; RINT(fp); ListeForeuse[i].placed = (bool)fp;
        RINT(ListeForeuse[i].q);
        ListeForeuse[i].texture = drillTexture;
    }

    // Machines processeurs — reconvertit les index en .id OpenGL courants
    #define LOAD_MACHINES(list, count) do { \
        RINT(count); \
        for (int i = 0; i < (count); i++) { \
            int ei, mi, fi, placed; \
            RINT((list)[i].i);  RINT((list)[i].j); \
            RINT(placed); (list)[i].placed = (bool)placed; \
            RINT((list)[i].energy_q);  RINT(ei); (list)[i].energy_id   = IdxToTex(ei).id; \
            RINT((list)[i].material_q);RINT(mi); (list)[i].material_id = IdxToTex(mi).id; \
            RINT((list)[i].final_q);   RINT(fi); (list)[i].final_id    = IdxToTex(fi).id; \
        } \
    } while(0)

    LOAD_MACHINES(ListeFurnace,   numFurnaces);
    LOAD_MACHINES(ListeHydraulic, numHydraulics);
    LOAD_MACHINES(ListeEttireuse, numEttireuses);
    #undef LOAD_MACHINES

    #define LOAD_GENERATORS(list, count) do { \
        RINT(count); \
        for (int i = 0; i < (count); i++) { \
            int ei, mi, placed; \
            RINT((list)[i].i);  RINT((list)[i].j); \
            RINT(placed); (list)[i].placed = (bool)placed; \
            RINT((list)[i].energy_q);  RINT(ei); (list)[i].energy_id   = IdxToTex(ei).id; \
            RINT((list)[i].material_q);RINT(mi); (list)[i].material_id = IdxToTex(mi).id; \
            RINT((list)[i].final_q); \
        } \
    } while(0)

    LOAD_GENERATORS(ListeSteam, numSteams);
    LOAD_GENERATORS(ListeOil,   numOils);
    #undef LOAD_GENERATORS

    for (int i = 0; i < MAX_CONVEYOR; i++) {
        int ttex, ttomove;
        RINT(ListeConveyor[i].i);  RINT(ListeConveyor[i].j);
        RINT(ListeConveyor[i].dir[0]);  RINT(ListeConveyor[i].dir[1]);
        int cp, cpo; RINT(cp); ListeConveyor[i].placed = (bool)cp;
        RINT(cpo); ListeConveyor[i].power = (bool)cpo;
        if (has_max_load) {
            RINT(ListeConveyor[i].max_load);
        }
        if (has_amount) {
            RINT(ListeConveyor[i].amount);
            RINT(ListeConveyor[i].capacity);
        } else {
            ListeConveyor[i].amount   = 0;
            ListeConveyor[i].capacity = 2;
        }
        RINT(ttex);    ListeConveyor[i].texture       = IdxToTex(ttex);
        if (!has_amount) {
            int cm; RINT(cm);  // inMouvement — champ supprimé, on lit pour avancer
        }
        RINT(ttomove); ListeConveyor[i].textureToMove = IdxToTex(ttomove);
        if (ListeConveyor[i].placed && ListeConveyor[i].max_load == 0 &&
            ListeConveyor[i].texture.id == piloneTexture.id)
            ListeConveyor[i].max_load = 5;
        if (ListeConveyor[i].placed && ListeConveyor[i].capacity == 0)
            ListeConveyor[i].capacity = 2;
        ListeConveyor[i].processed = false;  // jamais sérialisé, toujours reset
    }

    for (int i = 0; i < MAX_BATTERY; i++) {
        RINT(ListeBattery[i].i);  RINT(ListeBattery[i].j);
        RINT(ListeBattery[i].q);
        int bp; RINT(bp); ListeBattery[i].placed = (bool)bp;
        ListeBattery[i].texture = batteryTexture;
    }

    RINT(days);

    fclose(file);
    return 0;
}

#undef WINT
#undef RINT

void InitGame() {
    InitTexture();
    InitTextureRegistry();  // doit être appelé juste après InitTexture()
    InitGrid();
    InitMusic();
    InitInventory();  // Initialiser l'inventaire avec des textures et des quantités d'exemple
    InitBaseCraft();
    setPlayerCamera();
    isGameInitialized = true;  // Marquer le jeu comme initialisé
}

int main(void) {
    // Fenêtre temporaire pour pouvoir appeler GetMonitorWidth
    InitWindow(1280, 800, "Minc Corp Simulation");
    int monW = GetMonitorWidth(0);
    int monH = GetMonitorHeight(0);
    // Choisir la plus grande résolution qui tient dans le moniteur
    if      (monW >= 1920 && monH >= 1080) { screenWidth = 1920; screenHeight = 1080; }
    else if (monW >= 1440 && monH >=  900) { screenWidth = 1440; screenHeight =  900; }
    else                                   { screenWidth = 1280; screenHeight =  800; }
    if (screenWidth != 1280 || screenHeight != 800) {
        CloseWindow();
        InitWindow(screenWidth, screenHeight, "Minc Corp Simulation");
    }
    update_size();
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

    ButtonPlay(); // Initialiser le bouton Play

    SetTargetFPS(60);  // Définir la fréquence d'images cible

    const double convInterval = 0.25;
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (!shouldClose) {
        update_size();

        // ── UPDATE ──────────────────────────────────────────────────────────
        UpdateMusic();
        UpdateBattery();

        clock_gettime(CLOCK_MONOTONIC, &current);
        double elapsed = (current.tv_sec - start.tv_sec) +
                         (current.tv_nsec - start.tv_nsec) / 1e9;
        if (elapsed >= convInterval) {
            Update_Conv();
            clock_gettime(CLOCK_MONOTONIC, &start);
        }

        if (currentScreen == GAME) {
            Update_Foreuse();
            Update_Furnace();
            Update_Steam();
            Update_Oil();
            Update_Hydraulic();
            Update_Ettireuse();
        }

        // ── INPUT ───────────────────────────────────────────────────────────
        InitInventoryKeyBiding();

        if (IsKeyPressed(KEY_R)) UpdateDir();
        if (IsKeyPressed(KEY_TAB)) selectedItem = (selectedItem + 1) % 10;
        if (IsKeyPressed(KEY_F1)) DebugEnergy();

        if (currentScreen != MENU) {
            if (IsKeyPressed(KEY_E)) {
                isInventoryScreenOpen = !isInventoryScreenOpen;
                currentScreen = isInventoryScreenOpen ? INVENT : GAME;
            }
            if (IsKeyPressed(KEY_SEMICOLON)) {
                isMapScreenOpen = !isMapScreenOpen;
                currentScreen = isMapScreenOpen ? MAP : GAME;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                isOptionScreenOpen = !isOptionScreenOpen;
                currentScreen = isOptionScreenOpen ? OPTION : GAME;
            }
            if (IsKeyPressed(KEY_C)) {
                isCraftScreenOpen = !isCraftScreenOpen;
                currentScreen = isCraftScreenOpen ? CRAFT : GAME;
            }
        }

        // ── DRAW ────────────────────────────────────────────────────────────
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU) {
            DrawMenu(&currentScreen);
            if (currentScreen == GAME && !isGameInitialized)
                InitGame();
        } else if (currentScreen == INVENT) {
            DrawInventoryPage();
        } else if (currentScreen == OPTION) {
            DrawEscapePage();
        } else if (currentScreen == CRAFT) {
            DrawCraftPage();
        } else if (currentScreen == MAP) {
            DrawMap(DayAndNight());
        }

        BeginMode2D(camera);
        if (currentScreen == GAME) {
            CurrentScreenFix();
            GridDraw();
            rightClic();
            leftClic();
            mouseDefault();
            moveCamera();
            DrawMiniMap();
        }
        EndMode2D();

        if (currentScreen == GAME)
            DrawInventoryBar();

        EndDrawing();
    }

    // ── CLEANUP ─────────────────────────────────────────────────────────────
    UnloadMusic();
    UnloadAllTexture();
    CloseWindow();

    return 0;
}