#include "cell.h"
#include "inventory.h"
#include "camera.h"
#include "stdio.h"

Cell grid[ROW][COL];


int screenWidth = 1920;
int screenHeight = 1080;
int cellSize;

void update_size(){
    cellSize = screenWidth / COL;
}


Generator generators[MAX_GENERATORS];
Filon ListeFilon[NB_FILON];



int GetDirectionAngle(int direction[2]) {
    if (direction[0] == 1 && direction[1] == 0) return 0;   // Droite
    if (direction[0] == 0 && direction[1] == -1) return 270; // Bas
    if (direction[0] == -1 && direction[1] == 0) return 180; // Gauche
    if (direction[0] == 0 && direction[1] == 1) return 90;    // Haut
    return 0; // Erreur si la direction ne correspond pas
}
void CellDraw(Cell cell, int brightValue) {
    float rotation = GetDirectionAngle(cell.dir);
    Texture2D RefTexture = chestTexture;

    float scaleX = (float)cellSize / RefTexture.width;
    float scaleY = (float)cellSize / RefTexture.height;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;

    Rectangle destRec = {
        cell.i * cellSize + cellSize / 2.0f,
        cell.j * cellSize + cellSize / 2.0f,
        RefTexture.width  * scale,
        RefTexture.height * scale
    };
    Rectangle sourceRec = { 0, 0, (float)RefTexture.width, (float)RefTexture.height };
    Vector2   origin    = { destRec.width / 2.0f, destRec.height / 2.0f };

    DrawTexturePro(cell.texture, sourceRec, destRec, origin, 0.0f, WHITE);

    if (cell.up_texture.id != 0) {
        DrawTexturePro(cell.up_texture, sourceRec, destRec, origin, rotation, WHITE);
        if (cell.move_texture.id != 0 && cell.up_texture.id != defaultTexture.id)
            DrawTexturePro(cell.move_texture, sourceRec, destRec, origin, 0.0f, WHITE);
    }

    // Filtre nuit appliqué par cellule si brightValue > 0
    if (brightValue > 0) {
        Rectangle nightRec = {
            cell.i * cellSize,
            cell.j * cellSize,
            RefTexture.width  * 2,
            RefTexture.height * 2
        };
        DrawRectangleRec(nightRec, (Color){ 0, 0, 0, (unsigned char)brightValue });
    }
}


// Fonction pour vérifier si les indices de la grille sont valides
bool IndexIsValid(int i, int j) {
    return (i >= 0 && i < COL && j >= 0 && j < ROW);
}

int DayAndNight() {
    // Cache le résultat pour la frame courante — plusieurs appels dans la même frame
    // retournent la même valeur sans recalcul (ex: GridDraw + DrawMap).
    static float lastTime   = -1.0f;
    static int   cached     = 0;
    float currentTime = GetTime();
    if (currentTime == lastTime) return cached;
    lastTime = currentTime;

    int time       = (int)currentTime % 40;
    int maxOpacity = 64;
    int BrightValue;

    if      (time < 10) BrightValue = 0;
    else if (time < 20) BrightValue = (time - 10) * (maxOpacity / 10);
    else if (time < 30) BrightValue = maxOpacity;
    else                BrightValue = (40 - time) * (maxOpacity / 10);

    static int lastCycle = 0;
    int cycle = (int)currentTime / 40;
    if (cycle > lastCycle) {
        days += cycle - lastCycle;
        lastCycle = cycle;
    }

    cached = BrightValue < 0 ? 0 : (BrightValue > maxOpacity ? maxOpacity : BrightValue);
    return cached;
}

void InitGrid() {
    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            grid[i][j] = (Cell) {
                .i = i,
                .j = j,
                .placed = false,
                .pickable=true,
                .moveable=true,
                .isSolid=true,
                .dir={0,0},
                .texture = defaultTexture,  // Par défaut, on peut utiliser n'importe quelle texture def
                .up_texture = (Texture2D){ 0 } ,
                .move_texture= (Texture2D){ 0 }   

            };
        }
    }
    InitMineraiGenerator();
}
void InitMineraiGenerator() {
    ListeFilon[0] = (Filon){ .max_number=15,  .density=15,  .texture=copperVeinTexture, .up_texture=copperTexture,         .isSolid=true  };
    ListeFilon[1] = (Filon){ .max_number=20,  .density=10,  .texture=ironVeinTexture,   .up_texture=ironTexture,           .isSolid=true  };
    ListeFilon[2] = (Filon){ .max_number=50,  .density=20,  .texture=coalVeinTexture,   .up_texture=coalTexture,           .isSolid=true  };
    ListeFilon[3] = (Filon){ .max_number=15,  .density=20,  .texture=rockVeinTexture,   .up_texture=rockTexture,           .isSolid=true  };
    ListeFilon[4] = (Filon){ .max_number=100, .density=20,  .texture=sandVeinTexture,   .up_texture=sandDust,              .isSolid=true  };
    ListeFilon[5] = (Filon){ .max_number=150, .density=150, .texture=waterVeinTexture,  .up_texture=(Texture2D){0},        .isSolid=false };
    ListeFilon[6] = (Filon){ .max_number=15,  .density=20,  .texture=oilVeinTexture,    .up_texture=(Texture2D){0},        .isSolid=false };
    
    // Initialiser chaque générateur
    // srand initialisé dans main.c
    
    for (int k = 0; k < MAX_GENERATORS; k++) {
        int ore_type = (rand() % (sizeof(ListeFilon) / sizeof(ListeFilon[0])));
            generators[k] = (Generator){
                .max_number=ListeFilon[ore_type].max_number,
                .density=ListeFilon[ore_type].density,
                .i = rand()%((screenWidth-100)/cellSize) , // Position x initiale
                .j = rand()%((screenWidth-100)/cellSize), // Position y initiale
                .placed = false, // Initialisé comme placé
                .isSolid=ListeFilon[ore_type].isSolid,
                .texture = ListeFilon[ore_type].texture,
                .up_texture =ListeFilon[ore_type].up_texture};

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

        while (mineralsPlaced < generators[k].max_number && attempts < 10) { // Tenter de placer 3 minerais avec une limite de tentatives
            int dirIndex = rand() % 4;
            int newI = generators[k].i + directions[dirIndex][0];
            int newJ = generators[k].j + directions[dirIndex][1];

            for(int i = 0;i<generators[k].density;i++){ // étend les filons
                // Vérifier si la nouvelle position est valide et si la cellule n'est pas déjà occupée
                if (IndexIsValid(newI, newJ) && !grid[newI][newJ].placed) 
                {
                    // Placer le minerai
                    grid[newI][newJ].texture = generators[k].texture; // Utiliser la texture du générateur
                    //marque la case comme liquide ou non
                    grid[newI][newJ].isSolid = generators[k].isSolid;
                    mineralsPlaced++;
                    if(rand()%6 >2) {
                        grid[newI][newJ].up_texture = generators[k].up_texture;
                    }
                    else {
                        grid[newI][newJ].up_texture = (Texture2D){0};
                        }
                    if(grid[newI][newJ].up_texture.id != 0)grid[newI][newJ].placed=true;
    
                }
                dirIndex = rand() % 4 ;
                newI +=directions[dirIndex][0];
                newJ +=directions[dirIndex][1];
            }
            


            attempts++; // Incrémenter le compteur de tentatives
        }        
    }
}

void GridDraw() {
    Vector2 topLeft     = GetScreenToWorld2D((Vector2){ 0, 0 }, camera);
    Vector2 bottomRight = GetScreenToWorld2D((Vector2){ screenWidth, screenHeight }, camera);

    int startX = (int)(topLeft.x / cellSize);
    int startY = (int)(topLeft.y / cellSize);
    int endX   = (int)(bottomRight.x / cellSize);
    int endY   = (int)(bottomRight.y / cellSize);

    startX = startX < 0 ? 0 : startX;
    startY = startY < 0 ? 0 : startY;
    endX   = endX >= COL ? COL - 1 : endX;
    endY   = endY >= ROW ? ROW - 1 : endY;

    // Calculé UNE SEULE FOIS pour toute la frame
    int brightValue = DayAndNight();

    for (int i = startX; i <= endX; i++)
        for (int j = startY; j <= endY; j++)
            CellDraw(grid[i][j], brightValue);
}