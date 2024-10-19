#include "raylib.h"
#include "cell.h"
#include "inventory.h"
#include "camera.h"

// Taille de la zone de vision de la caméra
const int visibleCols = 50;
const int visibleRows = 50;

// Fonction pour mettre à jour la position de la caméra
void UpdateTheCamera() {
    // Déplacer la caméra avec les touches fléchées
    if (IsKeyDown(KEY_D)) offsetX += 5;
    if (IsKeyDown(KEY_A)) offsetX -= 5;
    if (IsKeyDown(KEY_W)) offsetY += 5;
    if (IsKeyDown(KEY_Z)) offsetY -= 5;

    // Limiter l'offset pour ne pas sortir de la carte
    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;

    int maxOffsetX = COL * cellSize - screenWidth;
    int maxOffsetY = ROW * cellSize - screenHeight;

    if (offsetX > maxOffsetX) offsetX = maxOffsetX;
    if (offsetY > maxOffsetY) offsetY = maxOffsetY;
}

// Fonction pour dessiner la carte en se concentrant sur les cases visibles
void DrawMap() {
    // Calculer les indices des colonnes et lignes visibles
    int startCol = offsetX / cellSize;
    int startRow = offsetY / cellSize;
    int endCol = startCol + visibleCols;
    int endRow = startRow + visibleRows;

    // S'assurer que les indices ne sortent pas des limites de la grille
    startCol = (startCol < 0) ? 0 : startCol;
    startRow = (startRow < 0) ? 0 : startRow;
    endCol = (endCol > COL) ? COL : endCol;
    endRow = (endRow > ROW) ? ROW : endRow;

    // Dessiner les cellules visibles
    for (int i = startCol; i < endCol; i++) {
        for (int j = startRow; j < endRow; j++) {
            Cell cell = grid[i][j];

            int drawX = cell.i * cellSize - offsetX;
            int drawY = cell.j * cellSize - offsetY;

            // Dessiner la texture de la cellule si elle est placée
            if (cell.placed) {
                int textureOffsetX = (cellSize - cell.texture.width) / 2;
                int textureOffsetY = (cellSize - cell.texture.height) / 2;
                DrawTexture(cell.texture, drawX + textureOffsetX, drawY + textureOffsetY, WHITE);
            }

            // Dessiner les bordures des cellules
            DrawRectangleLines(drawX, drawY, cellSize, cellSize, LIGHTGRAY);
        }
    }
}

// Fonction principale pour gérer la caméra et dessiner la carte
void CameraUpdateAndDraw() {
    UpdateTheCamera();  // Met à jour la position de la caméra
    DrawMap();       // Dessine la partie de la carte visible par la caméra
}
