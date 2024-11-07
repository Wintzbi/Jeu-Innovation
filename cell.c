#include "cell.h"
#include "inventory.h"
#include "camera.h"

int offsetX = 0;
int offsetY = 0;

Generator generators[MAX_GENERATORS];
Generator MineraiCuivreGenerator;
Generator MineraiFerGenerator;

int screenWidth;
int screenHeight;
int cellSize;

void InitScreenSize() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    cellSize = screenWidth / COL;
}

// Fonction pour dessiner une cellule
void CellDraw(Cell cell) {
    if (cell.placed) {
        int offsetX = (cellSize - cell.texture.width)/2;
        int offsetY = (cellSize - cell.texture.height)/2;
        DrawTexture(cell.texture, cell.i * cellSize + offsetX, cell.j * cellSize + offsetY, WHITE);
    }
    DrawRectangleLines(cell.i * cellSize, cell.j * cellSize, cellSize, cellSize, LIGHTGRAY);
}

// Fonction pour vérifier si les indices de la grille sont valides
bool IndexIsValid(int i, int j) {
    return (i >= 0 && i < COL && j >= 0 && j < ROW);
}

void InitGrid() {
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            grid[i][j] = (Cell) {
                .i = i,
                .j = j,
                .placed = false,
                .pickable=true,
                .texture = (Texture2D){ 0 }  // Par défaut, on peut utiliser n'importe quelle texture
            };
        }
    }
    InitMineraiGenerator();
}
void InitMineraiGenerator() {
    // Exemple de textures pour les générateurs
    Texture2D GeneratorTextures[] = {copperTexture, ironTexture}; // Ajoutez d'autres textures selon vos besoins

    // Initialiser chaque générateur
    srand(time(NULL)); // Utilise le temps actuel comme valeur de départ
    for (int k = 0; k < MAX_GENERATORS; k++) {
            generators[k] = (Generator){
                .i = rand()%30, // Position x initiale
                .j = 5+rand()%20, // Position y initiale
                .placed = true, // Initialisé comme placé
                .texture = GeneratorTextures[rand() % (sizeof(GeneratorTextures) / sizeof(GeneratorTextures[0]))]
            };
            grid[generators[k].i][generators[k].j].texture = generators[k].texture ;
            grid[generators[k].i][generators[k].j].placed = generators[k].placed ;
            grid[generators[k].i][generators[k].j].pickable=false;
    } 
    
    MineraiGenerator();
}

void MineraiGenerator() {
    int directions[4][2] = {
        {-1, 0}, // Haut
        {1, 0},  // Bas
        {0, -1}, // Gauche
        {0, 1}   // Droite
    };

    for (int k = 0; k < MAX_GENERATORS; k++) {
        int mineralsPlaced = 0;
        int attempts = 0; // Compteur pour les tentatives

        while (mineralsPlaced < 3 && attempts < 10) { // Tenter de placer 3 minerais avec une limite de tentatives
            int dirIndex = rand() % 4; // Choisir une direction aléatoire
            int newI = generators[k].i + directions[dirIndex][0];
            int newJ = generators[k].j + directions[dirIndex][1];

            // Vérifier si la nouvelle position est valide et si la cellule n'est pas déjà occupée
            if (IndexIsValid(newI, newJ) && !grid[newI][newJ].placed) {
                // Placer le minerai
                grid[newI][newJ].texture = generators[k].texture; // Utiliser la texture du générateur
                grid[newI][newJ].placed = true; // Marquer la cellule comme occupée
                mineralsPlaced++;
            } 

            attempts++; // Incrémenter le compteur de tentatives
        }        
    }
}



void GridDraw() {
    // Calculer les limites visibles de la vue de la caméra
    Vector2 topLeft = GetScreenToWorld2D((Vector2){ 0, 0 }, camera);
    Vector2 bottomRight = GetScreenToWorld2D((Vector2){ screenWidth, screenHeight }, camera);

    // Calculer la grille visibles les coordonnées
    int startX = (int)(topLeft.x / cellSize);
    int startY = (int)(topLeft.y / cellSize);
    int endX = (int)(bottomRight.x / cellSize);
    int endY = (int)(bottomRight.y / cellSize);

    // Vérifier que cela fait bien parti de la grille
    startX = startX < 0 ? 0 : startX;
    startY = startY < 0 ? 0 : startY;
    endX = endX >= COL ? COL - 1 : endX;
    endY = endY >= ROW ? ROW - 1 : endY;

    // Dessiner uniquement les cases visibles
    for (int i = startX; i <= endX; i++) {
        for (int j = startY; j <= endY; j++) {
            CellDraw(grid[i][j]);
        }
    }
}
