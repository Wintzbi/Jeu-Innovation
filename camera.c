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
    // On laisse en x0.75 pour le moment pitié
    if (screenWidth == 1920*0.5 && screenHeight == 1200*0.5) {
        minX = 160.0f*0.5;           // Bord gauche
        maxX = 640.0f;        // Bord droit
        minY = 100.0f*0.5;           // Bord en haut
        maxY = 700.0f;        // Bord en bas
    }

    if (screenWidth == 1920 && screenHeight == 1080) {
        minX = 160.0f;           // Bord gauche
        maxX = 1640;        // Bord droit
        minY = 100.0f;           // Bord en haut
        maxY = 1700;        // Bord en bass
    }

    if (screenWidth == 1920*0.75 && screenHeight == 1200*0.75) {
        minX = 160.0f*0.75;           // Bord gauche
        maxX = 1280.0f;        // Bord droit
        minY = 100.0f*0.75;           // Bord en haut
        maxY = 1325.0f;        // Bord en bas
    }
}


void setPlayerCamera() {
    camera.target = (Vector2){0.0f, 0.0f}; // Position de la cible (initialement à l'origine)
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f}; // Centre la caméra au milieu de l'écran
    camera.rotation = 0.0f;
    camera.zoom = 6.0f;

    // Configure la mini-carte
    miniMapCamera = camera;
    miniMapCamera.zoom = 0.3f; // Zoom différent pour la mini-carte

    InitBounds(screenWidth, screenHeight);
}

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
    //  cameraSpeed += mouseWheelMove * 0.1f;
   // }

    // Limite le zoom
    //if (camera.zoom < 1.2f) camera.zoom = 1.2f; // Min zoom
    //if (camera.zoom > 3.0f) camera.zoom = 3.0f; // Max zoom

    // Limiter la caméra
    if (camera.target.x < minX) camera.target.x = minX;
    if (camera.target.x > maxX) camera.target.x = maxX;
    if (camera.target.y < minY) camera.target.y = minY;
    if (camera.target.y > maxY) camera.target.y = maxY;

    // Debug
    //printf("Camera position: (%.2f, %.2f), zoom: %.2f\n", camera.target.x, camera.target.y, camera.zoom);
}

// Ne marche pas
void DrawMiniMap() {
    int miniMapWidth = screenWidth / 2;
    int miniMapHeight = screenHeight / 2;
    int miniMapX = screenWidth - miniMapWidth - 5;
    int miniMapY = screenHeight - miniMapHeight - 10;

    // Dessine la vue de la mini-carte

    // BeginScissorMode(miniMapX, miniMapY, miniMapWidth, miniMapHeight);
    // BeginMode2D(miniMapCamera);
    // GridDraw();  // Dessiner la grille de jeu
    // EndMode2D();
    // EndScissorMode();

    // Dessiner un cadre autour de la mini-carte

  
}
