#include "cell.h"
#include "inventory.h"

const int screenWidth = 1920*0.5;
const int screenHeight = 1200*0.5;
const int cellSize = screenWidth / COL;
int offsetX = 0;
int offsetY = 0;
Generator MineraiCuivreGenerator;

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

void InitGrid() {
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            grid[i][j] = (Cell) {
                .i = i,
                .j = j,
                .placed = false,
                .texture = copperTexture  // Par défaut, on peut utiliser n'importe quelle texture
            };
        }
    }
    InitMineraiGenerator();
}
void InitMineraiGenerator(){
    MineraiCuivreGenerator=(Generator){
        .i = 25,
        .j = 25,
        .placed = true,
        .texture = copperTexture
    };
    grid[MineraiCuivreGenerator.i][MineraiCuivreGenerator.j].texture = MineraiCuivreGenerator.texture ;
    grid[MineraiCuivreGenerator.i][MineraiCuivreGenerator.j].placed = MineraiCuivreGenerator.placed ;
    MineraiGenerator();
}
void MineraiGenerator() {
    int directions[4][2] = {
        {-1, 0}, // Haut
        {1, 0},  // Bas
        {0, -1}, // Gauche
        {0, 1}   // Droite
    };

    int mineralsPlaced = 0;
    while (mineralsPlaced < 3) { // Tenter de placer 3 minerais
        int dirIndex = rand() % 4; // Choisir une direction aléatoire
        int newI = MineraiCuivreGenerator.i + directions[dirIndex][0];
        int newJ = MineraiCuivreGenerator.j + directions[dirIndex][1];

        // Vérifier si la nouvelle position est valide
        if (newI >= 0 && newI < COL && newJ >= 0 && newJ < ROW && !grid[newI][newJ].placed) {
            // Placer le minerai
            grid[newI][newJ].texture = copperTexture; // Vous pouvez changer le type de minerai si nécessaire
            grid[newI][newJ].placed = true;
            mineralsPlaced++;
            printf("Mineral placed at (%d, %d)\n", newI, newJ);
        }
    }
}

void GridDraw() {
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            CellDraw(grid[i][j]);
        }
    }
}