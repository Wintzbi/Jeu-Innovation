#include "raylib.h"
#include "cell.h"
#include "camera.h"
#include <stdio.h>

const float cameraSpeed = 200.0f;
Camera2D camera;

void setPlayerCamera() {
    camera.target = (Vector2){ 0.0f, 0.0f }; // Position de la cible (initialement à l'origine)
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Centre la caméra au milieu de l'écran
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

void moveCamera() {
    if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * GetFrameTime();

        // Gestion du zoom de la caméra avec la molette de la souris
    if (IsKeyDown(KEY_RIGHT)) camera.zoom += 0.005f;
    if (IsKeyDown(KEY_LEFT)) camera.zoom -= 0.005f;
    
        // Limiter le zoom pour éviter des comportements imprévus
    if (camera.zoom < 0.1f) camera.zoom = 1.0f;
    if (camera.zoom > 3.0f) camera.zoom = 3.0f;
}