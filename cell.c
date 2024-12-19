#include "cell.h"
#include "inventory.h"
#include "camera.h"
#include "stdio.h"


int screenWidth = 1920;
int screenHeight = 1080;
int cellSize;

void update_size(){
    cellSize = screenWidth / COL;
}

int offsetX = 0;
int offsetY = 0;
float rotation;
int BrightValue = 0;

Generator generators[MAX_GENERATORS];
Filon ListeFilon[NB_FILON];
Filon CopperVein;
Filon IronVein;
Filon CoalVein;
Filon RockVein;
Filon SandVein;
Filon WaterVein;
Filon OilVein;
Generator MineraiCuivreGenerator;
Generator MineraiFerGenerator;
Generator MineraiCoalGenerator;
Generator MineraiRockGenerator;
Generator MineraiSandGenerator;



int GetDirectionAngle(int direction[2]) {
    if (direction[0] == 1 && direction[1] == 0) return 0;   // Droite
    if (direction[0] == 0 && direction[1] == -1) return 270; // Bas
    if (direction[0] == -1 && direction[1] == 0) return 180; // Gauche
    if (direction[0] == 0 && direction[1] == 1) return 90;    // Haut
    return 0; // Erreur si la direction ne correspond pas
}
void DrawMap(){
    for(int r=0;r<ROW;r++){
        for (int c = 0; c < COL; c++)
        {
            Cell cell=grid[r][c];

            float rotation = GetDirectionAngle(cell.dir); // Récupérer l'angle de direction pour la rotation
            Texture2D RefTexture = chestTexture; // Définir la texture de référence
            // Calcul de l'échelle pour adapter la texture à la taille de la cellule
            float scaleX = (float)cellSize*0.50 / RefTexture.width;
            float scaleY = (float)cellSize*0.50/ RefTexture.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY; // Garder les proportions

            // Rectangle de destination : position centrée dans la cellule
            Rectangle destRec = {
                (screenWidth/4) +cell.i * cellSize*0.50 + cellSize*0.50 / 2.0f, // Centre de la cellule en X
                100+cell.j * cellSize*0.50 + cellSize*0.50 / 2.0f, // Centre de la cellule en Y
                RefTexture.width * scale,           // Largeur ajustée avec l'échelle
                RefTexture.height * scale           // Hauteur ajustée avec l'échelle
            };
            // Rectangle source de la texture (portion de la texture à dessiner)
            Rectangle sourceRec = { 0, 0, (float)RefTexture.width, (float)RefTexture.height};

            // Origine pour la rotation (centre du rectangle de destination)
            Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

            DrawTexturePro(cell.texture, sourceRec, destRec, origin, 0.0f, WHITE);
            if (cell.up_texture.id != 0) {
                DrawTexturePro(cell.up_texture, sourceRec, destRec, origin, rotation, WHITE);
            }

            // Dessiner la texture en mouvement si elle est valide
            if (cell.move_texture.id != 0) {
                DrawTexturePro(cell.move_texture, sourceRec, destRec, origin, 0.0f, WHITE);
            }
        }
        
    }
    DrawRectangle((screenWidth/2)-400, 10, 800, 60, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("MAP"), (screenWidth/2)-390, 11, 60, Fade(WHITE,0.7f));
    if (screenWidth == 1920 && screenHeight == 1080) {
        DrawRectangleLines((screenWidth/4) +( camera.target.x/2)-80, 100 + (camera.target.y/2)-50, (cellSize*0.54)*ROW/camera.zoom, (cellSize*0.33)*COL/camera.zoom, RED);
    }

    if (screenWidth == 1920*0.75 && screenHeight == 1200*0.75) {
        DrawRectangleLines((screenWidth/4) +( camera.target.x/2)-60, 100 + (camera.target.y/2)-40, (cellSize*0.5)*ROW/camera.zoom, (cellSize*0.35)*COL/camera.zoom, RED);
    }


    Rectangle screenRec = {
         (screenWidth/4),                // Position X (coin supérieur gauche de l'écran)
        100,                // Position Y (coin supérieur gauche de l'écran)
        cellSize*0.50*ROW,      // Largeur du rectangle (taille de l'écran)
        cellSize*0.50*COL      // Hauteur du rectangle (taille de l'écran)
    };

    int BrightValue= DayAndNight();
    // Ajouter un filtre noir semi-transparent
    Color nightFilter = { 0, 0, 0,BrightValue }; // Assurez-vous que `value` est entre 0 et `maxOpacity`
    DrawRectangleRec(screenRec, nightFilter); 
     
    DrawText(TextFormat("CREDITS : "),10,100,35, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Jeu créé durant le "),10,140,25, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("parcours Innovation, "),10,170,25, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("du premier semestre"),10,200,25, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("2024 par : "),10,230,25, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Leynaïck Duthieuw,"),10,260,25, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Charlilou Maes"),10,290,25, Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Mathys Karin-Grisoni"),10,320,25, Fade(DARKGRAY,0.7f));



    DrawText(TextFormat("CONTROLS :"),(cellSize*0.50*ROW)+ (screenWidth/4)+10, 100 ,35,Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Déplacements : ZQSD"),(cellSize*0.50*ROW)+ (screenWidth/4)+10, 140 ,25,Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Option : ESC"),(cellSize*0.50*ROW)+ (screenWidth/4)+10, 170 ,25,Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Inventaire : E"),(cellSize*0.50*ROW)+ (screenWidth/4)+10, 200 ,25,Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Craft : C"),(cellSize*0.50*ROW)+ (screenWidth/4)+10, 230 ,25,Fade(DARKGRAY,0.7f));
    DrawText(TextFormat("Map : M"),(cellSize*0.50*ROW)+ (screenWidth/4)+10, 260 ,25,Fade(DARKGRAY,0.7f));


}

void CellDraw(Cell cell) {
    float rotation = GetDirectionAngle(cell.dir); // Récupérer l'angle de direction pour la rotation
    Texture2D RefTexture = chestTexture; // Définir la texture de référence

    // Calcul de l'échelle pour adapter la texture à la taille de la cellule
    float scaleX = (float)cellSize*1.0 / RefTexture.width;
    float scaleY = (float)cellSize*1.0 / RefTexture.height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY; // Garder les proportions

    // Rectangle de destination : position centrée dans la cellule
    Rectangle destRec = { 
        cell.i * cellSize + cellSize / 2.0f, // Centre de la cellule en X
        cell.j * cellSize + cellSize / 2.0f, // Centre de la cellule en Y
        RefTexture.width * scale,           // Largeur ajustée avec l'échelle
        RefTexture.height * scale           // Hauteur ajustée avec l'échelle
    };


    // Rectangle source de la texture (portion de la texture à dessiner)
    Rectangle sourceRec = { 0, 0, (float)RefTexture.width, (float)RefTexture.height };

    // Origine pour la rotation (centre du rectangle de destination)
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

    // Dessiner la texture principale
    DrawTexturePro(cell.texture, sourceRec, destRec, origin, 0.0f, WHITE);

    // Dessiner la texture "up" si elle est valide
    if (cell.up_texture.id != 0) {
        DrawTexturePro(cell.up_texture, sourceRec, destRec, origin, rotation, WHITE);
        if (cell.move_texture.id != 0 && cell.up_texture.id !=defaultTexture.id) {
        DrawTexturePro(cell.move_texture, sourceRec, destRec, origin, 0.0f, WHITE);
    }
    }

    // Dessiner la texture en mouvement si elle est valide

    //On abandonne la minimap trop relou
    //DrawMiniMapVersion(cell);
    // Dessiner les contours de la cellule
   // DrawRectangleLines(cell.i * cellSize, cell.j * cellSize, cellSize*1.5, cellSize*1.5, Fade(LIGHTGRAY,0.5f));
       
    // Rectangle pour le filtre
    Rectangle nightRec = { 
        cell.i * cellSize,         // Centre de la cellule en X
        cell.j * cellSize,         // Centre de la cellule en Y
        RefTexture.width * 2,      // Largeur ajustée avec l'échelle
        RefTexture.height * 2      // Hauteur ajustée avec l'échelle
    };
    int BrightValue= DayAndNight();
    // Ajouter un filtre noir semi-transparent
    Color nightFilter = { 0, 0, 0,BrightValue }; // Assurez-vous que `value` est entre 0 et `maxOpacity`
    DrawRectangleRec(nightRec, nightFilter);
}


// Fonction pour vérifier si les indices de la grille sont valides
bool IndexIsValid(int i, int j) {
    return (i >= 0 && i < COL && j >= 0 && j < ROW);
}

int DayAndNight(){
 float currentTime = GetTime();
    int time = (int)currentTime % 40;  // Temps cyclique entre 0 et 39 secondes
    int maxOpacity = 64;  // Opacité maximale de 128

    // Phase de jour (entre 0 et 10 secondes)
    if (time < 10) {
        // L'opacité reste à 0 pendant la journée
        BrightValue = 0;  
    }
    // Phase de montée vers la nuit (entre 10 et 20 secondes)
    else if (time >= 10 && time < 20) {
        // L'opacité augmente progressivement de 0 à 128
        BrightValue = (time - 10) * (maxOpacity / 10);  // L'opacité atteint `maxOpacity` à la fin de la montée vers la nuit (20 secondes)
    }
    // Phase de nuit (entre 20 et 30 secondes)
    else if (time >= 20 && time < 30) {
        // L'opacité reste à `maxOpacity` pendant la nuit
        BrightValue = maxOpacity;  // L'opacité reste à 128 pendant la nuit
    }
    // Phase de descente vers le jour (entre 30 et 40 secondes)
    else if (time >= 30 && time < 40) {
        // L'opacité diminue progressivement de 128 à 0
        BrightValue = (40 - time) * (maxOpacity / 10);  // L'opacité descend à 0 à la fin de la descente vers le jour (40 secondes)
    }

    if (time==40) days ++;
    return  Clamp(BrightValue, 0, maxOpacity);
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
     CopperVein=(Filon) {
        .max_number=15,
        .density=15,
        .texture =copperVeinTexture,
        .up_texture=copperTexture,
        .isSolid=true
    };
    IronVein=(Filon) {
        .max_number=20,
        .density=10,
        .texture =ironVeinTexture,
        .up_texture=ironTexture,
        .isSolid=true
    };
    CoalVein=(Filon) {
        .max_number=50,
        .density=20,
        .texture =coalVeinTexture,
        .up_texture=coalTexture,
        .isSolid=true
    };
    RockVein=(Filon) {
        .max_number=15,
        .density=20,
        .texture =rockVeinTexture,
        .up_texture=rockTexture,
        .isSolid=true
    };
    SandVein=(Filon) {
        .max_number=100,
        .density=20,
        .texture =sandVeinTexture,
        .up_texture=sandDust,
        .isSolid=true
    };
    WaterVein=(Filon) {
        .max_number=150,
        .density=150,
        .texture =waterVeinTexture,
        .up_texture=(Texture){0},
        .isSolid=false
    };
    OilVein=(Filon) {
        .max_number=15,
        .density=20,
        .texture =oilVeinTexture,
        .up_texture=(Texture){0},
        .isSolid=false
    };
    ListeFilon[0]= CopperVein;
    ListeFilon[1]= IronVein;
    ListeFilon[2]= CoalVein;
    ListeFilon[3]= RockVein;
    ListeFilon[4]= SandVein;
    ListeFilon[5]= WaterVein;
    ListeFilon[6]= OilVein;
    
    // Initialiser chaque générateur
    srand(time(NULL)); // Utilise le temps actuel comme valeur de départ
    
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
            int dirIndex = rand() % 5; // Choisir une direction aléatoire
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