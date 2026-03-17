#include "raylib.h"
#include "cell.h"
#include "camera.h"
#include <stdio.h>

const float cameraSpeed = 100.0f;
Camera2D camera;
Camera2D miniMapCamera;

float minX;
float maxX;
float minY;
float maxY;

void InitBounds(int screenWidth, int screenHeight) {
    float gridW = COL * (float)cellSize;
    float gridH = ROW * (float)cellSize;
    float halfW = screenWidth  / (2.0f * camera.zoom);
    float halfH = screenHeight / (2.0f * camera.zoom);
    minX = halfW;
    maxX = gridW - halfW;
    minY = halfH;
    maxY = gridH - halfH;
}

// À appeler si le zoom change en jeu
void UpdateBounds() {
    InitBounds(screenWidth, screenHeight);
}

void setPlayerCamera() {
    camera.target   = (Vector2){0.0f, 0.0f};
    camera.offset   = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom     = 6.0f;
    miniMapCamera   = camera;
    miniMapCamera.zoom = 0.3f;
    InitBounds(screenWidth, screenHeight);
}

void moveCamera() {
    if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * GetFrameTime();

    // Zoom molette — décommente et appelle UpdateBounds() après si tu l'actives
    // float wheel = GetMouseWheelMove();
    // if (wheel != 0) { camera.zoom += wheel * 0.5f; UpdateBounds(); }

    if (camera.target.x < minX) camera.target.x = minX;
    if (camera.target.x > maxX) camera.target.x = maxX;
    if (camera.target.y < minY) camera.target.y = minY;
    if (camera.target.y > maxY) camera.target.y = maxY;
}

void DrawMiniMap() {
    // Non implémenté — réservé pour une future minimap ScissorMode
}