#include "cell.h"
#include "inventory.h"
#include "camera.h"

const int screenWidth = 1920*0.75;
const int screenHeight = 1200*0.75;
const int cellSize = screenWidth / COL;
int offsetX = 0;
int offsetY = 0;

Generator generators[MAX_GENERATORS];
Foreuse ListeForeuse[MAX_FOREUSES];
Foreuse foreuse;

Generator MineraiCuivreGenerator;
Generator MineraiFerGenerator;

// Fonction pour dessiner une cellule avec texture ajustée et centrée
// Fonction pour dessiner une cellule avec texture ajustée et centrée, y compris la texture supplémentaire
void CellDraw(Cell cell) {
        Texture2D RefTexture;
        RefTexture = defaultTexture;        // Définir la texture de référence (RefTexture) en fonction de la validité de up_texture
        

        float scaleX = (float)cellSize / RefTexture.width;
        float scaleY = (float)cellSize / RefTexture.height;
        float scale = (scaleX < scaleY) ? scaleX : scaleY; // Choisir le plus petit pour garder les proportions

        Rectangle destRec = { 
            cell.i * cellSize + (cellSize - RefTexture.width * scale) / 2, 
            cell.j * cellSize + (cellSize - RefTexture.height * scale) / 2, 
            RefTexture.width * scale, 
            RefTexture.height * scale 
        };

        Rectangle sourceRec = { 0, 0, (float)RefTexture.width, (float)RefTexture.height };
        Vector2 origin = { 0, 0 };

        DrawTexturePro(cell.texture, sourceRec, destRec, origin, 0.0f, WHITE);
        

    
        // Dessine la texture supplémentaire si elle est valide (id != 0)
        if (cell.up_texture.id != 0) {
            DrawTexturePro(cell.up_texture, sourceRec, destRec, origin, 0.0f, WHITE);
        }
        
    // Dessiner les contours de la cellule
    DrawRectangleLines(cell.i * cellSize, cell.j * cellSize, cellSize * 1.25, cellSize * 1.25, LIGHTGRAY);
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
                .texture = defaultTexture,  // Par défaut, on peut utiliser n'importe quelle texture
                .up_texture = (Texture2D){ 0 }  

            };
        }
    }
    InitMineraiGenerator();
}
void InitMineraiGenerator() {
    // Exemple de textures pour les générateurs
    Texture2D GeneratorVeinTextures[] = {copperVeinTexture, ironVeinTexture}; // Ajoutez d'autres textures selon vos besoins
    Texture2D GeneratorOreTextures[] = {copperTexture, ironTexture}; // Ajoutez d'autres textures selon vos besoins

    // Initialiser chaque générateur
    srand(time(NULL)); // Utilise le temps actuel comme valeur de départ
    for (int k = 0; k < MAX_GENERATORS; k++) {
        int ore_type = rand() % (sizeof(GeneratorVeinTextures) / sizeof(GeneratorVeinTextures[0]));
            generators[k] = (Generator){
                .i = rand()%60, // Position x initiale
                .j = 10 +rand()%40, // Position y initiale
                .placed = false, // Initialisé comme placé
                .texture = GeneratorVeinTextures[ore_type],
                .up_texture =GeneratorOreTextures[ore_type]};

            grid[generators[k].i][generators[k].j].texture = generators[k].texture ;
            grid[generators[k].i][generators[k].j].placed = generators[k].placed ;
            grid[generators[k].i][generators[k].j].pickable=true;
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

        while (mineralsPlaced < 10 && attempts < 10) { // Tenter de placer 3 minerais avec une limite de tentatives
            int dirIndex = rand() % 5; // Choisir une direction aléatoire
            int newI = generators[k].i + directions[dirIndex][0];
            int newJ = generators[k].j + directions[dirIndex][1];

            for(int i = 0;i<10;i++){ // étend les filons
                // Vérifier si la nouvelle position est valide et si la cellule n'est pas déjà occupée
                if (IndexIsValid(newI, newJ) && !grid[newI][newJ].placed) 
                {
                    // Placer le minerai
                    grid[newI][newJ].texture = generators[k].texture; // Utiliser la texture du générateur
                    grid[newI][newJ].placed = true; // Marquer la cellule comme occupée
                    mineralsPlaced++;
                    if(rand()%6 >2) grid[newI][newJ].up_texture = generators[k].up_texture;
                    else {
                        grid[newI][newJ].up_texture = (Texture2D){0};
                        grid[newI][newJ].placed=false;
                        }
    
                }
                dirIndex = rand() % 4 ;
                newI +=directions[dirIndex][0];
                newJ +=directions[dirIndex][1];
            }
            


            attempts++; // Incrémenter le compteur de tentatives
        }        
    }
}

void InteractForeuse(){
    for (int k = 0; k < MAX_FOREUSES; k++){
        if (ListeForeuse[k].placed){
            continue;
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