#include "saveload.h"
#include "global.h"
#include "energy.h"
#include "texture_registry.h"
#include <stdio.h>

#define WINT(v)  { int _v=(v); fwrite(&_v, sizeof(int), 1, file); }
#define RINT(v)  fread(&(v), sizeof(int), 1, file)

static int RawIdToIdx(unsigned int rawId) {
    if (rawId == 0) return TEX_NONE;
    Texture2D t = { .id = rawId };
    return TexToIdx(t);
}

int save(void) {
    FILE *file = fopen("save.dat", "wb");
    if (!file) { perror("save"); return 1; }

    int version = 5;
    WINT(version);

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

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fwrite(inventory[i].name, sizeof(char), 20, file);
        WINT(TexToIdx(inventory[i].texture));
        WINT(inventory[i].quantity);
    }

    WINT(numForeuses);
    for (int i = 0; i < numForeuses; i++) {
        WINT(ListeForeuse[i].i);  WINT(ListeForeuse[i].j);
        WINT((int)ListeForeuse[i].placed);
        WINT(ListeForeuse[i].q);
    }

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

    for (int i = 0; i < MAX_BATTERY; i++) {
        WINT(ListeBattery[i].i);  WINT(ListeBattery[i].j);
        WINT(ListeBattery[i].q);
        WINT((int)ListeBattery[i].placed);
    }

    WINT(days);
    fclose(file);
    return 0;
}

int load(void) {
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
        if (has_max_load) RINT(ListeConveyor[i].max_load);
        if (has_amount) {
            RINT(ListeConveyor[i].amount);
            RINT(ListeConveyor[i].capacity);
        } else {
            ListeConveyor[i].amount   = 0;
            ListeConveyor[i].capacity = 2;
        }
        RINT(ttex);    ListeConveyor[i].texture       = IdxToTex(ttex);
        if (!has_amount) { int cm; RINT(cm); }
        RINT(ttomove); ListeConveyor[i].textureToMove = IdxToTex(ttomove);
        if (ListeConveyor[i].placed && ListeConveyor[i].max_load == 0 &&
            ListeConveyor[i].texture.id == piloneTexture.id)
            ListeConveyor[i].max_load = 5;
        if (ListeConveyor[i].placed && ListeConveyor[i].capacity == 0)
            ListeConveyor[i].capacity = 2;
        ListeConveyor[i].processed = false;
    }

    for (int i = 0; i < MAX_BATTERY; i++) {
        RINT(ListeBattery[i].i);  RINT(ListeBattery[i].j);
        RINT(ListeBattery[i].q);
        int bp; RINT(bp); ListeBattery[i].placed = (bool)bp;
        ListeBattery[i].texture = batteryTexture;
    }

    RINT(days);
    RebuildSolarPanels();
    fclose(file);
    return 0;
}

#undef WINT
#undef RINT
