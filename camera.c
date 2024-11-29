#include "raylib.h"
#include "cell.h"
#include "camera.h"
#include "texture.h"
#include <stdio.h>

float cameraSpeed = 100.0f;
Camera2D camera;

float minX;
float maxX;
float minY;
float maxY;

void InitBounds(int screenWidth, int screenHeight) {
    if (screenWidth == 1920*0.5 && screenHeight == 1200*0.5) {
        minX = 160.0f*0.5;           // Bord gauche
        maxX = 640.0f;        // Bord droit
        minY = 100.0f*0.5;           // Bord en haut
        maxY = 700.0f;        // Bord en bas
    }

    if (screenWidth == 1920 && screenHeight == 1200) {
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
    camera.target = (Vector2){ 0.0f, 0.0f }; // Position de la cible (initialement à l'origine)
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Centre la caméra au milieu de l'écran
    camera.rotation = 0.0f;
    camera.zoom = 6.0f;
    InitBounds(screenWidth,screenHeight);
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

    float mouseWheelMove = GetMouseWheelMove();
    if (mouseWheelMove != 0) {
      cameraSpeed += mouseWheelMove * 0.1f;
    }

    // Limite le zoom
    //if (camera.zoom < 1.0f) camera.zoom = 1.0f; // Min zoom
    //if (camera.zoom > 3.0f) camera.zoom = 3.0f; // Max zoom

    // Limiter la caméra
    if (camera.target.x < minX) camera.target.x = minX;
    if (camera.target.x > maxX) camera.target.x = maxX;
    if (camera.target.y < minY) camera.target.y = minY;
    if (camera.target.y > maxY) camera.target.y = maxY;

    // Debug
    printf("Camera position: (%.2f, %.2f), zoom: %.2f\n", camera.target.x, camera.target.y, camera.zoom);
}

void drawMiniMap(int miniMapWidth, int miniMapHeight) {
    // Position de la mini-carte dans la carte globale
    Vector2 miniMapStart = { minX, minY };
    Vector2 miniMapEnd = { maxX, maxY };

    // Échelle pour ajuster les coordonnées de la grille dans la mini-carte
    float scaleX = (float)miniMapWidth / (miniMapEnd.x - miniMapStart.x);
    float scaleY = (float)miniMapHeight / (miniMapEnd.y - miniMapStart.y);

    // Parcourir toutes les cellules de la carte
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            // Calculer la position de la cellule dans la mini-carte
            float cellX = (grid[i][j].i - miniMapStart.x) * scaleX;
            float cellY = (grid[i][j].j - miniMapStart.y) * scaleY;
            float cellSizeX = cellSize * scaleX;
            float cellSizeY = cellSize * scaleY;

            // Dessiner la cellule dans la mini-carte
            DrawRectangle(cellX, cellY, cellSizeX, cellSizeY, GREEN);
        }
    }
}

void miniMap(int screenWidth, int screenHeight) {
    // Dimensions de la mini-carte
    const int miniMapWidth = 200;
    const int miniMapHeight = 150;
    const int miniMapX = screenWidth - miniMapWidth - 50;
    const int miniMapY = screenHeight - miniMapHeight - 30;

    // Dessiner la grille à l'intérieur du rectangle de la mini-carte
    drawMiniMap(miniMapWidth, miniMapHeight);

    // Dessiner le cadre rouge pour la position actuelle de la caméra
    float mapWidth = maxX - minX;
    float mapHeight = maxY - minY;

    float scaleX = (float)miniMapWidth / mapWidth;
    float scaleY = (float)miniMapHeight / mapHeight;

    float cameraRectX = miniMapX + (camera.target.x - minX) * scaleX;
    float cameraRectY = miniMapY + (camera.target.y - minY) * scaleY;

    float cameraRectWidth = (screenWidth / camera.zoom) * scaleX;
    float cameraRectHeight = (screenHeight / camera.zoom) * scaleY;

    DrawRectangleLines(miniMapX, miniMapY, miniMapWidth+cameraRectWidth, miniMapHeight+cameraRectHeight, WHITE); 
    DrawRectangleLines(cameraRectX, cameraRectY, cameraRectWidth, cameraRectHeight, RED);
}
