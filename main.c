<<<<<<< HEAD
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define COL 50
#define ROW 50
#define INVENTORY_SIZE 10

const int screenWidth = 1920;
const int screenHeight = 1200;

const int cellSize = screenWidth / COL;
bool itemFound = false;

int offsetX = 0;
int offsetY =0;

typedef struct Cell {
    int i;
    int j;
    bool placed;
    Texture2D texture;
} Cell;

typedef struct Item {
    Texture2D texture;
    int quantity;
} Item;

Cell grid[ROW][COL];
Item inventory[INVENTORY_SIZE];  // Inventaire avec 3 types de blocs
int selectedItem = 0;  // L'index de l'item sélectionné dans l'inventaire

// Fonction pour dessiner une cellule
void CellDraw(Cell cell) {
    if (cell.placed) {
        int offsetX = (cellSize - cell.texture.width) / 2;
        int offsetY = (cellSize - cell.texture.height) / 2;
        DrawTexture(cell.texture, cell.i * cellSize + offsetX, cell.j * cellSize + offsetY, WHITE);
    }
    DrawRectangleLines(cell.i * cellSize, cell.j * cellSize, cellSize, cellSize, LIGHTGRAY);
}

// Fonction pour vérifier si les indices de la grille sont valides
bool IndexIsValid(int i, int j) {
    return (i >= 0 && i < COL && j >= 0 && j < ROW);
}

// Fonction pour dessiner l'inventaire avec des images agrandies
void DrawInventory() {
    int invWidth = screenWidth;
    int invHeight = 50;  // Fixed height for inventory
    int slotSize = invWidth / INVENTORY_SIZE;  // Size of each inventory slot

    // Size of textures in the inventory
    int targetImageSize = 32;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        // Draw the inventory slot with DARKGRAY
        Color slotColor = Fade(DARKGRAY, 0.9f);
        DrawRectangle(i * slotSize, 0, slotSize, invHeight, slotColor);
        DrawRectangleLines(i * slotSize, 0, slotSize, invHeight, BLACK);

        // Highlight the selected slot
        if (i == selectedItem) {
            Rectangle borderRect = { (float)(i * slotSize), 0, (float)slotSize, (float)invHeight };
            DrawRectangleLinesEx(borderRect, 4, Fade(LIGHTGRAY, 0.8f));  // Draw border
        }

        // Check if the quantity is > 0 to draw the enlarged texture
        if (inventory[i].quantity > 0) {
            Rectangle source = { 0, 0, inventory[i].texture.width, inventory[i].texture.height };
            Rectangle dest = { i * slotSize + (slotSize - targetImageSize) / 2, 
                               (invHeight - targetImageSize) / 2, 
                               targetImageSize, targetImageSize };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(inventory[i].texture, source, dest, origin, 0.0f, WHITE);
            DrawText(TextFormat("%d", inventory[i].quantity), i * slotSize + 25, 10, 20, WHITE); // Display quantity
            itemFound = true;
        } else {
            itemFound = false;
        }
    }
}


int main(void) {
    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");

    // Charger les textures des items
    InitTexture();
    InitGrid();

    // Initialisation de l'inventaire avec des textures et des quantités d'exemple
    inventory[0] = (Item) { copperTexture, 2 };  // Bloc de cuivre avec 2 unités
    inventory[1] = (Item) { ironTexture, 2 };    // Bloc de fer avec 2 unités
    inventory[2] = (Item) { coalTexture, 20 };   // Bloc de charbon avec 20 unités
    inventory[3] = (Item) {};
    inventory[4] = (Item) {};
    inventory[5] = (Item) {};
    inventory[6] = (Item) {};
    inventory[7] = (Item) {};
    inventory[8] = (Item) {};
    inventory[9] = (Item) {};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        int posX = (int)(GetMousePosition().x / cellSize);
        int posY = (int)(GetMousePosition().y / cellSize);

        // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)
        if (IsKeyPressed(KEY_ONE)) selectedItem = 0;
        if (IsKeyPressed(KEY_TWO)) selectedItem = 1;
        if (IsKeyPressed(KEY_THREE)) selectedItem = 2;
        if (IsKeyPressed(KEY_FOUR)) selectedItem = 3;
        if (IsKeyPressed(KEY_FIVE)) selectedItem = 4;
        if (IsKeyPressed(KEY_SIX)) selectedItem = 5;
        if (IsKeyPressed(KEY_SEVEN)) selectedItem = 6;
        if (IsKeyPressed(KEY_EIGHT)) selectedItem = 7;
        if (IsKeyPressed(KEY_NINE)) selectedItem = 8;
        if (IsKeyPressed(KEY_ZERO)) selectedItem = 9;

        // Placer un bloc avec un clic droit
        rightClic();
=======
#include "global.h"
//test pull
int main(void) {

    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");
    // Charger les textures des items
    InitTexture();

    // Inititaliser la grille
    InitGrid();

    // Initialisation de l'inventaire avec des textures et des quantités d'exemple
    InitInventory();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Sélectionner l'item avec les touches numériques (1, 2, 3, ...)
        InitInventoryKeyBiding();

        // Placer un bloc avec un clic droit
        rightClic();
        // Le récupérer grâce au clil gauche
>>>>>>> 564ce8022679a86fbc78daac1dbfff4b59414fdc
        leftClic();

        // Début du dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner la grille
        GridDraw();

<<<<<<< HEAD
        mouseDefault();

        // Dessiner l'inventaire
        DrawInventory();
=======
        // Afficher la souris par défault
        mouseDefault();
>>>>>>> 564ce8022679a86fbc78daac1dbfff4b59414fdc

        // Dessiner l'inventaire
        DrawInventory();
        EndDrawing();
    }

    // Déchargement des textures
    UnloadAllTexture();
<<<<<<< HEAD

=======
>>>>>>> 564ce8022679a86fbc78daac1dbfff4b59414fdc
    CloseWindow();

    return 0;
}