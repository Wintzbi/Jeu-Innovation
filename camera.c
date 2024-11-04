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

const float minX = 320.0f;           // Bord gauche
const float maxX = 1480.0f;        // Bord droit
const float minY = 200.0f;           // Bord en haut
const float maxY = 1600.0f;        // Bord en bas

void moveCamera() {
    // Gère les mouvements de la caméra
    if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * GetFrameTime();
    if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * GetFrameTime();

    // Gestion du zoom de la caméra avec les touches ou la molette de la souris
    //if (IsKeyDown(KEY_RIGHT)) camera.zoom += 0.005f;
    //if (IsKeyDown(KEY_LEFT)) camera.zoom -= 0.005f;

    //float mouseWheelMove = GetMouseWheelMove();
    //if (mouseWheelMove != 0) {
        //camera.zoom += mouseWheelMove * 0.1f;
    //}

    // Limite le zoom
    if (camera.zoom < 1.2f) camera.zoom = 1.2f; // Min zoom
    if (camera.zoom > 3.0f) camera.zoom = 3.0f; // Max zoom

    // Limiter la caméra
    if (camera.target.x < minX) camera.target.x = minX;
    if (camera.target.x > maxX) camera.target.x = maxX;
    if (camera.target.y < minY) camera.target.y = minY;
    if (camera.target.y > maxY) camera.target.y = maxY;

    // Debug
    printf("Camera position: (%.2f, %.2f), zoom: %.2f\n", camera.target.x, camera.target.y, camera.zoom);
}
