#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define COL 30
#define ROW 30

const int screenWidth = 1000;
const int screenHeight = 800;
const int cellSize = screenWidth / COL;

typedef struct Cell {
    int i;
    int j;
    bool placed;
    Color color;
} Cell;

typedef struct Machine {
    Color color;
} Machine;

Cell grid[ROW][COL];

Machine machine;
Color colors[] = {RED, GREEN, BLUE}; 
int colorIndex = 0; 

void CellDraw(Cell cell) {
    if (cell.placed) {
        DrawRectangle(cell.i * cellSize, cell.j * cellSize, cellSize, cellSize, cell.color);
    }
    DrawRectangleLines(cell.i * cellSize, cell.j * cellSize, cellSize, cellSize, LIGHTGRAY);
}

bool IndexIsValid(int i, int j) {
    return (i >= 0 && i < COL && j >= 0 && j < ROW);
}

int main(void) {
    // Initialisation de la fenêtre
    InitWindow(screenWidth, screenHeight, "Minc Corp simulation");

    // Initialisation de la machine avec la première couleur
    machine.color = colors[colorIndex];

    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            grid[i][j] = (Cell) {
                .i = i,
                .j = j,
                .placed = false,
                .color = WHITE // Couleur par défaut pour les cellules non placées
            };
        }
    }

    SetTargetFPS(60);  // Limite à 60 FPS

    // Boucle principale du jeu
    while (!WindowShouldClose()) { // Tant que l'utilisateur ne ferme pas la fenêtre

        int posX = (int)(GetMousePosition().x / cellSize);
        int posY = (int)(GetMousePosition().y / cellSize);

        // Changer la couleur de la machine en faisant défiler la molette de la souris
        if (GetMouseWheelMove() < 0) {
            colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
        }
        // Si la molette est tournée vers le haut
        if (GetMouseWheelMove() > 0) {
            colorIndex = (colorIndex - 1 + (sizeof(colors) / sizeof(colors[0]))) % (sizeof(colors) / sizeof(colors[0]));
        }
        machine.color = colors[colorIndex];

        // Réinitialiser les clics sur les cellules lorsque le bouton est enfoncé
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (IndexIsValid(posX, posY)) {
                grid[posX][posY].placed = false;
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            if (IndexIsValid(posX, posY)) {
                grid[posX][posY].placed = true;
                grid[posX][posY].color = machine.color; // Assignez la couleur actuelle
            }
        }

        // Début du dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner le rectangle de survol avec la couleur actuelle de la machine
        DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(machine.color, 0.3f));

        // Dessiner le cadrillage
        for (int i = 0; i < COL; i++) {
            for (int j = 0; j < ROW; j++) {
                CellDraw(grid[i][j]);
            }
        }

        EndDrawing();
    }

    // Fermeture de la fenêtre et nettoyage
    CloseWindow();

    return 0;
}
