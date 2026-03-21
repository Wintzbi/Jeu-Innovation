#include "global.h"
#include "saveload.h"
#include "energy.h"
#include "production.h"
#include "conveyor.h"
#include "texture_registry.h"

GameScreen currentScreen       = MENU;
bool isInventoryScreenOpen     = false;
bool isOptionScreenOpen        = false;
bool isCraftScreenOpen         = false;
bool isMapScreenOpen           = false;
bool isGameInitialized         = false;

void CurrentScreenFix(void) {
    if (currentScreen == GAME) {
        isInventoryScreenOpen = false;
        isOptionScreenOpen    = false;
        isCraftScreenOpen     = false;
        isMapScreenOpen       = false;
    }
}

void InitGame(void) {
    InitTexture();
    InitTextureRegistry();
    InitGrid();
    InitMusic();
    InitInventory();
    InitBaseCraft();
    setPlayerCamera();
    isGameInitialized = true;
}

int main(void) {
    InitWindow(1280, 800, "Minc Corp Simulation");
    int monW = GetMonitorWidth(0);
    int monH = GetMonitorHeight(0);
    if      (monW >= 1920 && monH >= 1080) { screenWidth = 1920; screenHeight = 1080; }
    else if (monW >= 1440 && monH >=  900) { screenWidth = 1440; screenHeight =  900; }
    else                                   { screenWidth = 1280; screenHeight =  800; }
    if (screenWidth != 1280 || screenHeight != 800) {
        CloseWindow();
        InitWindow(screenWidth, screenHeight, "Minc Corp Simulation");
    }
    update_size();
    srand(time(NULL));
    ButtonPlay();
    SetTargetFPS(60);

    const double convInterval = 0.25;
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (!shouldClose) {
        update_size();

        // ── UPDATE ────────────────────────────────────────────────────────
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

        // ── INPUT ─────────────────────────────────────────────────────────
        InitInventoryKeyBiding();
        if (IsKeyPressed(KEY_R))   UpdateDir();
        if (IsKeyPressed(KEY_TAB)) selectedItem = (selectedItem + 1) % 10;
        if (IsKeyPressed(KEY_F1))  DebugEnergy();
        if (IsKeyPressed(KEY_F2))  infoMode = !infoMode;

        if (currentScreen != MENU) {
            if (IsKeyPressed(KEY_E))         { isInventoryScreenOpen = !isInventoryScreenOpen; currentScreen = isInventoryScreenOpen ? INVENT : GAME; }
            if (IsKeyPressed(KEY_SEMICOLON)) { isMapScreenOpen       = !isMapScreenOpen;       currentScreen = isMapScreenOpen       ? MAP    : GAME; }
            if (IsKeyPressed(KEY_ESCAPE))    { isOptionScreenOpen    = !isOptionScreenOpen;    currentScreen = isOptionScreenOpen    ? OPTION : GAME; }
            if (IsKeyPressed(KEY_C))         { isCraftScreenOpen     = !isCraftScreenOpen;     currentScreen = isCraftScreenOpen     ? CRAFT  : GAME; }
        }

        // ── DRAW ──────────────────────────────────────────────────────────
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU) {
            DrawMenu(&currentScreen);
            if (currentScreen == GAME && !isGameInitialized) InitGame();
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
            DrawInfoTooltip();
        }
        EndMode2D();

        if (currentScreen == GAME) DrawInventoryBar();

        EndDrawing();
    }

    UnloadMusic();
    UnloadAllTexture();
    CloseWindow();
    return 0;
}
