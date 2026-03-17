#include "menu.h"
#include "cell.h"
#include "camera.h"

Rectangle playButton, LargeButton, LilButton;

// Résolutions proposées — filtrées au runtime selon la taille du moniteur
static const int RES_W[] = { 1280, 1440, 1920 };
static const int RES_H[] = {  800,  900, 1080 };
static const char* RES_LABEL[] = { "1280x800", "1440x900", "1920x1080" };
#define NUM_RES 3

static Rectangle resButtons[NUM_RES];
static int       numResButtons = 0;

void ButtonPlay() {
    int cx = screenWidth  / 2;
    int cy = screenHeight / 2;

    playButton = (Rectangle){ cx - 75, cy - 30, 150, 55 };

    // Ne garder que les résolutions qui tiennent dans le moniteur
    int monW = GetMonitorWidth(0);
    int monH = GetMonitorHeight(0);
    numResButtons = 0;
    for (int i = 0; i < NUM_RES; i++) {
        if (RES_W[i] <= monW && RES_H[i] <= monH)
            resButtons[numResButtons++] = (Rectangle){0, 0, 160, 50};
    }

    // Centrer les boutons horizontalement sous Play
    int totalW = numResButtons * 160 + (numResButtons - 1) * 20;
    int startX = cx - totalW / 2;
    for (int i = 0; i < numResButtons; i++) {
        resButtons[i].x = startX + i * 180;
        resButtons[i].y = cy + 60;
    }
}

static void ApplyResolution(int w, int h) {
    screenWidth  = w;
    screenHeight = h;
    CloseWindow();
    InitWindow(screenWidth, screenHeight, "Minc Corp Simulation");
    update_size();
    setPlayerCamera();
    ButtonPlay();
}

void DrawMenu(GameScreen *currentScreen) {
    int cx = screenWidth  / 2;
    int cy = screenHeight / 2;

    // Titre centré
    const char *title = "Minc Corp Simulation";
    int titleW = MeasureText(title, 40);
    DrawText(title, cx - titleW / 2, cy - 120, 40, DARKGRAY);

    // Bouton Play
    Color playColor = CheckCollisionPointRec(GetMousePosition(), playButton) ? LIGHTGRAY : GRAY;
    DrawRectangleRec(playButton, playColor);
    const char *playLabel = "Play";
    int playLW = MeasureText(playLabel, 25);
    DrawText(playLabel,
             (int)(playButton.x + playButton.width  / 2) - playLW / 2,
             (int)(playButton.y + playButton.height / 2) - 12,
             25, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), playButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *currentScreen = GAME;
    }

    // Boutons résolution
    for (int i = 0; i < numResButtons; i++) {
        Color c = CheckCollisionPointRec(GetMousePosition(), resButtons[i]) ? LIGHTGRAY : GRAY;
        DrawRectangleRec(resButtons[i], c);
        int lw = MeasureText(RES_LABEL[i], 18);
        DrawText(RES_LABEL[i],
                 (int)(resButtons[i].x + resButtons[i].width  / 2) - lw / 2,
                 (int)(resButtons[i].y + resButtons[i].height / 2) - 9,
                 18, WHITE);
        if (CheckCollisionPointRec(GetMousePosition(), resButtons[i]) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ApplyResolution(RES_W[i], RES_H[i]);
        }
    }
}