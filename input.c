#include "cell.h"
#include "input.h"
#include "inventory.h"
#include "camera.h"
#include <stdio.h>
#include <string.h> // Pour strcmp

int MinPlaceableID = 23; // Liste des ID de textures plaçables
Conveyor ListeConveyor[MAX_CONVEYOR];
bool inMouvement = false;
Foreuse ListeForeuse[MAX_FOREUSE];
int conveyor_dir=0;
int option =0;
int numForeuses = 0;
float lastForeuseTime;

Furnace ListeFurnace[MAX_FURNACE];
int numFurnaces = 0;
float lastFurnaceTime;

Hydraulic ListeHydraulic[MAX_HYDRAULIC];
int numHydraulics = 0;
float lastHydraulicTime;

Ettireuse ListeEttireuse[MAX_ETTIREUSE];
int numEttireuses = 0;
float lastEttireuseTime;


Steam ListeSteam[MAX_STEAM];
int numSteams = 0;  // Compteur de foreuses
float lastSteamTime;

Oil ListeOil[MAX_OIL];
int numOils = 0;  // Compteur de foreuses
float lastOilTime;


Battery ListeBattery[MAX_BATTERY];

int directions[4][2] = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1} 
    };

void mouseDefault() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (inventory[selectedItem].texture.id != 0 && inventory[selectedItem].texture.id >= MinPlaceableID) {
        Rectangle destRect = { posX * cellSize + cellSize / 2, posY * cellSize + cellSize / 2, cellSize, cellSize };
        Rectangle srcRect = { 0, 0, inventory[selectedItem].texture.width, inventory[selectedItem].texture.height };
        Vector2 origin = { cellSize / 2, cellSize / 2 };

        DrawTexturePro(inventory[selectedItem].texture, srcRect, destRect, origin, 0.0f, Fade(WHITE, 0.3f));
    }
    else DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(RED, 0.3f));
}
    


void InitInventoryKeyBiding() {
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
}

void interraction(int posX, int posY);

void rightClic() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {

        interraction(posX,posY);
        
        if (inventory[selectedItem].quantity > 0 &&
                   inventory[selectedItem].texture.id >= MinPlaceableID) {
            if (IndexIsValid(posX, posY) && !grid[posX][posY].placed) {
                grid[posX][posY].placed = true;
                grid[posX][posY].up_texture = inventory[selectedItem].texture;
                printf("Name: %s | Texture: %d\n", inventory[selectedItem].name, inventory[selectedItem].texture.id);
                ActionWithName(inventory[selectedItem].name, posX, posY,option);
                inventory[selectedItem].quantity--;

                if (inventory[selectedItem].quantity == 0) {
                    inventory[selectedItem].texture = (Texture2D){0};
                }
            }
        }
    }
}

void RemoveForeuse(int posX, int posY);
void RemoveFurnace(int posX, int posY);


void leftClic() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (IndexIsValid(posX, posY) && grid[posX][posY].placed && grid[posX][posY].pickable &&
            grid[posX][posY].up_texture.id != 0) {
            Texture2D brokenTexture = grid[posX][posY].up_texture;
            bool itemFoundInInventory = false;

            for (int i = 0; i < INVENTORY_SIZE; i++) {
                if (inventory[i].texture.id == brokenTexture.id) {
                    inventory[i].quantity++;
                    itemFoundInInventory = true;
                    break;
                }
            }

            if (!itemFoundInInventory) {
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i].quantity == 0) {
                        inventory[i].texture = brokenTexture;
                        inventory[i].quantity = 1;
                        break;
                    }
                }
            }

            grid[posX][posY].up_texture = (Texture2D){0};
            grid[posX][posY].placed = false;
            grid[posX][posY].dir[0] = 0;
            grid[posX][posY].dir[1] = 0;
            pickedObject++;

            // Désactiver une foreuse si elle est présente
            for (int f = 0; f < numForeuses; f++) {
                if (ListeForeuse[f].i == posX && ListeForeuse[f].j == posY && ListeForeuse[f].placed) {
                    ListeForeuse[f].placed = false;
                    RemoveForeuse(ListeForeuse[f].i, ListeForeuse[f].j);
                    break;
                }
            }
            // Désactiver une foreuse si elle est présente
            for (int f = 0; f < MAX_BATTERY; f++) {
                if (ListeBattery[f].i == posX && ListeBattery[f].j == posY && ListeBattery[f].placed) {
                    ListeBattery[f].placed = false;
                    RemoveBattery(ListeBattery[f].i, ListeBattery[f].j);
                    break;
                }
            }
            // Désactiver un tapis si il est présent
            for (int f = 0; f < MAX_CONVEYOR; f++) {
                if (ListeConveyor[f].i == posX && ListeConveyor[f].j == posY && ListeConveyor[f].placed) {
                    ListeConveyor[f].placed = false;
                    grid[posX][posY].dir[0] = 0;
                    grid[posX][posY].dir[1] = 0;
                    grid[posX][posY].move_texture=(Texture2D){0};
                    RemoveConveyor(ListeConveyor[f].i, ListeConveyor[f].j);
                    break;
                }
            }
            
        }
    }
}
void UpdateDir(){
    conveyor_dir = (conveyor_dir + 1) % 4;
    printf("changement de direction, option : %d\n", conveyor_dir);
}

void ActionWithName(char ObjectName[20], int i, int j, int option) {
    if (strcmp(ObjectName, "Tapis") == 0) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                grid[i][j].dir[0] = directions[conveyor_dir][0];
                grid[i][j].dir[1] = directions[conveyor_dir][1];
                ListeConveyor[k] = (Conveyor){.i = i, .j = j, .texture = conveyorTexture, .dir = {directions[conveyor_dir][0], directions[conveyor_dir][1]}, .placed = true, .inMouvement = false, .textureToMove = (Texture2D){0}};
                break;
            }
        }
    } else if (strcmp(ObjectName, "Tuyau") == 0) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                grid[i][j].dir[0] = directions[conveyor_dir][0];
                grid[i][j].dir[1] = directions[conveyor_dir][1];
                ListeConveyor[k] = (Conveyor){.i = i, .j = j, .texture = pipeTexture, .dir = {directions[conveyor_dir][0], directions[conveyor_dir][1]}, .placed = true, .inMouvement = false, .textureToMove = (Texture2D){0}};
                grid[i][j].moveable = false;
                break;
            }
        }
    } else if (strcmp(ObjectName, "Pilone") == 0) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                grid[i][j].dir[0] = directions[conveyor_dir][0];
                grid[i][j].dir[1] = directions[conveyor_dir][1];
                ListeConveyor[k] = (Conveyor){.i = i, .j = j, .texture = piloneTexture, .dir = {directions[conveyor_dir][0], directions[conveyor_dir][1]}, .placed = true, .inMouvement = false, .textureToMove = (Texture2D){0}};
                break;
            }
        }
    } else if (strcmp(ObjectName, "Batterie") == 0) {
        for (int k = 0; k < MAX_BATTERY; k++) {
            if (!ListeBattery[k].placed) {
                ListeBattery[k] = (Battery){.i = i, .j = j, .texture = batteryTexture, .q = 0, .placed = true};
                break;
            }
        }
    } else if (strcmp(ObjectName, "Foreuse") == 0) {
        if (numForeuses < MAX_FOREUSE) {
            ListeForeuse[numForeuses++] = (Foreuse){.i = i, .j = j, .q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (strcmp(ObjectName, "Furnace") == 0) {
        if (numFurnaces < MAX_FURNACE) {
            ListeFurnace[numFurnaces++] = (Furnace){.i = i, .j = j, .energy_q = 0, .energy_id = 0, .material_id = 0, .material_q = 0, .final_id = 0, .final_q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (strcmp(ObjectName, "Hydraulic") == 0) {
        if (numHydraulics < MAX_HYDRAULIC) {
            ListeHydraulic[numHydraulics++] = (Hydraulic){.i = i, .j = j, .energy_q = 0, .energy_id = 0, .material_id = 0, .material_q = 0, .final_id = 0, .final_q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (strcmp(ObjectName, "Ettireuse") == 0) {
        if (numEttireuses < MAX_ETTIREUSE) {
            ListeEttireuse[numEttireuses++] = (Ettireuse){.i = i, .j = j, .energy_q = 0, .energy_id = 0, .material_id = 0, .material_q = 0, .final_id = 0, .final_q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (strcmp(ObjectName, "Centrale Vapeur") == 0) {
        if (numSteams < MAX_STEAM) {
            ListeSteam[numSteams++] = (Steam){.i = i, .j = j, .energy_q = 0, .energy_id = 0, .material_id = 0, .material_q = 0, .final_q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    } else if (strcmp(ObjectName, "Centrale Pétrole") == 0) {
        if (numOils < MAX_OIL) {
            ListeOil[numOils++] = (Oil){.i = i, .j = j, .energy_q = 0, .energy_id = 0, .material_id = 0, .material_q = 0, .final_q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    }
}



void Update_Conv() {
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (ListeConveyor[k].placed) {
            if (ListeConveyor[k].texture.id==piloneTexture.id){
                // pilone d'énergie
                if(IsEnergieNear(ListeConveyor[k].i, ListeConveyor[k].j,3)){
                    ListeConveyor[k].textureToMove = piloneEffectTexture;
                    ListeConveyor[k].placed=true;
                    grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture= ListeConveyor[k].textureToMove;
                }
                
            }
            else {
                Convey(&ListeConveyor[k]);
            } 
        }
    }
}

void UpdateBattery(){
    for (int k = 0; k < MAX_BATTERY; k++) {
        if (ListeBattery[k].placed) {
                if(IsEnergieNear(ListeBattery[k].i, ListeBattery[k].j,1) && ListeBattery[k].q<=100){
                    ListeBattery[k].q++;
                    printf("Batterie chargée à %d\n",ListeBattery[k].q);
                }
        }
}
}
void Convey(Conveyor *conv) {
    
    int srcI = conv->i - conv->dir[0];  // Calcul de la case source
    int srcJ = conv->j - conv->dir[1];  // Calcul de la case source
    int destI = conv->i + conv->dir[0]; // Calcul de la case destination
    int destJ = conv->j + conv->dir[1]; // Calcul de la case destination

    if (!IndexIsValid(srcI, srcJ) || !IndexIsValid(destI, destJ)) {
        return; // Eviter l'accès aux indices invalides
    }

    // Vérifier si un objet est prêt à être pris (case source)
    if (grid[srcI][srcJ].moveable &&
        grid[srcI][srcJ].up_texture.id != 0 && 
        grid[srcI][srcJ].up_texture.id != conv->texture.id) {
        if((grid[srcI][srcJ].isSolid && conv->texture.id==conveyorTexture.id) || (!grid[srcI][srcJ].isSolid && conv->texture.id==pipeTexture.id)){
            // Prendre l'objet de la case source
            conv->textureToMove = grid[srcI][srcJ].up_texture;
            grid[srcI][srcJ].placed = false;
            grid[srcI][srcJ].up_texture = (Texture2D){ 0 }; // Effacer la case source
            grid[conv->i][conv->j].move_texture=conv->textureToMove;
        }
    }
    else if(grid[srcI][srcJ].up_texture.id == drillTexture.id) {
        //Trouve la foreuse connecté
            for (int k = 0; k < numForeuses; k++) {
                if (ListeForeuse[k].i == srcI && ListeForeuse[k].j == srcJ) {
                    if (ListeForeuse[k].q>0){
                        Texture2D under_texture = grid[ListeForeuse[k].i][ListeForeuse[k].j].texture;
                        Texture2D mined_texture =(Texture2D){0};
                        if(conv->texture.id == conveyorTexture.id && grid[srcI][srcJ].isSolid){ //solid
                            if (under_texture.id == copperVeinTexture.id) {
                                mined_texture = copperTexture;
                            }
                            else if (under_texture.id == ironVeinTexture.id) {
                                mined_texture = ironTexture;
                            }
                            else if (under_texture.id == coalVeinTexture.id) {
                                mined_texture = coalTexture;
                            } 
                        }
                        else if(conv->texture.id == pipeTexture.id ){ //liquide
                            if (under_texture.id == waterVeinTexture.id) {
                                mined_texture = waterVeinTexture;
                            }
                            if (under_texture.id == oilVeinTexture.id) {
                                mined_texture = oilVeinTexture;
                            }
                            }
                        conv->textureToMove = mined_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeForeuse[k].q--;
                    }
                    
                }
            }
    }
    else if(grid[srcI][srcJ].up_texture.id == furnaceTexture.id ) {
        //Trouve le four connecté
            for (int k = 0; k < numForeuses; k++) {
                if (ListeFurnace[k].i == srcI && ListeFurnace[k].j == srcJ) {
                    if (ListeFurnace[k].final_q>0){
                        int crafted_textureId = ListeFurnace[k].final_id;
                        Texture2D crafted_texture =(Texture2D){0};
                        if (crafted_textureId == copperLingotTexture.id) {
                            crafted_texture = copperLingotTexture;
                        }
                        else if (crafted_textureId == ironLingotTexture.id) {
                            crafted_texture = ironLingotTexture;
                        }
                       
                        conv->textureToMove = crafted_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeFurnace[k].final_q--;
                    }
                    
                }
            }
    }


else if(grid[srcI][srcJ].up_texture.id == pressTexture.id ) {
        //Trouve la presse connecté
            for (int k = 0; k < numHydraulics; k++) {
                if (ListeHydraulic[k].i == srcI && ListeHydraulic[k].j == srcJ) {
                    if (ListeHydraulic[k].final_q>0){
                        int crafted_textureId = ListeHydraulic[k].final_id;
                        Texture2D crafted_texture =(Texture2D){0};

                        if (crafted_textureId == ironPlateTexture.id) {
                            crafted_texture = ironPlateTexture;
                        }
                        
                        else if (crafted_textureId == copperPlateTexture.id) {
                            crafted_texture = copperPlateTexture;
                        }

                       
                        conv->textureToMove = crafted_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeHydraulic[k].final_q--;
                    }
                    
                }
            }
    }
    
    else if(grid[srcI][srcJ].up_texture.id == stretchTexture.id ) {
        //Trouve l'étireuse connecté
            for (int k = 0; k < numEttireuses; k++) {
                if (ListeEttireuse[k].i == srcI && ListeEttireuse[k].j == srcJ) {
                    printf("Etirreuse trouvé, nombre de craft :  %d \n",ListeEttireuse[k].final_q);
                    if (ListeEttireuse[k].final_q>0){
                        int crafted_textureId = ListeEttireuse[k].final_id;
                        Texture2D crafted_texture =(Texture2D){0};

                        if (crafted_textureId == copperRodTexture.id) {
                            crafted_texture = copperRodTexture;
                        }
                        else if (crafted_textureId == ironRodTexture.id) {
                            crafted_texture = ironRodTexture;
                        }
                       
                        conv->textureToMove = crafted_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeEttireuse[k].final_q--;
                    }
                    
                }
            }
    }


    
    // Si en mouvement, vérifier la destination
    if (grid[srcI][srcJ].move_texture.id != 0 && grid[conv->i][conv->j].move_texture.id==0  ) {
        conv->textureToMove=grid[srcI][srcJ].move_texture;
        grid[srcI][srcJ].move_texture = (Texture2D){ 0 }; // Réinitialiser move_texture
        grid[conv->i][conv->j].move_texture=conv->textureToMove;
        }
    // Déposer l'objet au sol
    if (grid[destI][destJ].up_texture.id !=conv->texture.id && !grid[destI][destJ].placed && grid[conv->i][conv->j].move_texture.id!=0) {
        // grid[destI][destJ].placed = true;
        // grid[destI][destJ].up_texture = conv->textureToMove;
        // grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; // Réinitialiser move_texture

        // // Réinitialiser l'objet et l'état du convoyeur
        // conv->textureToMove = (Texture2D){ 0 };
    }
    else if (grid[destI][destJ].up_texture.id !=conv->texture.id && !grid[destI][destJ].placed){
         conv->textureToMove=grid[srcI][srcJ].move_texture;
         grid[conv->i][conv->j].move_texture=conv->textureToMove;
    }
    //Ajoute à l'inventaire
    else if (grid[destI][destJ].up_texture.id == chestTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        AddInInvent(1, conv->textureToMove);
        grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
        conv->textureToMove = (Texture2D){ 0 };
    }
    //ajoute au four
    else if (grid[destI][destJ].up_texture.id == furnaceTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numForeuses; k++) {
                if (ListeFurnace[k].i == destI && ListeFurnace[k].j == destJ) {
                    //ajoute du matériau si il correspond
                        if (ListeFurnace[k].material_id == conv->textureToMove.id && conv->textureToMove.id != coalTexture.id ) {
                            ListeFurnace[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        //si pas de matériaux on l'ajoute
                        else if (ListeFurnace[k].material_q ==0 && conv->textureToMove.id != coalTexture.id ) {
                            ListeFurnace[k].material_id = conv->textureToMove.id;
                            ListeFurnace[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeFurnace[k].energy_id == conv->textureToMove.id && conv->textureToMove.id == coalTexture.id ) {
                            ListeFurnace[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                        else if (ListeFurnace[k].energy_q == 0 && conv->textureToMove.id == coalTexture.id ) {
                            ListeFurnace[k].energy_id= conv->textureToMove.id;
                            ListeFurnace[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                }
            }
    }
    //ajoute a la presse
    else if (grid[destI][destJ].up_texture.id == pressTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numHydraulics; k++) {
                if (ListeHydraulic[k].i == destI && ListeHydraulic[k].j == destJ) {
                    printf("Ajout à la presse\n");
                    //ajoute du matériau si il correspond
                        if (ListeHydraulic[k].material_id == conv->textureToMove.id && conv->textureToMove.id != coalTexture.id ) {
                            ListeHydraulic[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        //si pas de matériaux on l'ajoute
                        else if (ListeHydraulic[k].material_q ==0 && conv->textureToMove.id != coalTexture.id ) {
                            ListeHydraulic[k].material_id = conv->textureToMove.id;
                            ListeHydraulic[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }      
                }
            }
    }
    //ajoute à l'étireuse
    else if (grid[destI][destJ].up_texture.id == stretchTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numEttireuses; k++) {
                if (ListeEttireuse[k].i == destI && ListeEttireuse[k].j == destJ) {
                    
                    //ajoute du matériau si il correspond
                        if (ListeEttireuse[k].material_id == conv->textureToMove.id && conv->textureToMove.id != 0 ) {
                            ListeEttireuse[k].material_q++;
                            printf("Ajout à l'étireuse id :%d, lingot : %d\n",conv->textureToMove.id,copperLingotTexture.id);
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        //si pas de matériaux on l'ajoute
                        else if (ListeEttireuse[k].material_q ==0 && conv->textureToMove.id != 0 ) {
                            ListeEttireuse[k].material_id = conv->textureToMove.id;
                            ListeEttireuse[k].material_q++;
                            printf("Ajout à l'étireuse id :%d, lingot : %d\n",conv->textureToMove.id,copperLingotTexture.id);
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }      
                }
            }
    }
    //ajoute a la centrale
    else if (grid[destI][destJ].up_texture.id == steamcentralTexture.id && grid[conv->i][conv->j].move_texture.id!=0){
        for (int k = 0; k < numSteams; k++) {
                if (ListeSteam[k].i == destI && ListeSteam[k].j == destJ) {
                    //ajoute du matériau si il correspond
                        if (ListeSteam[k].material_id == conv->textureToMove.id && conv->textureToMove.id != coalTexture.id ) {
                            ListeSteam[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        //si pas de matériaux on l'ajoute
                        else if (ListeSteam[k].material_q ==0 && conv->textureToMove.id != coalTexture.id ) {
                            ListeSteam[k].material_id = conv->textureToMove.id;
                            ListeSteam[k].material_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        }
                        else if (ListeSteam[k].energy_id == conv->textureToMove.id && (conv->textureToMove.id == coalTexture.id  || conv->textureToMove.id == oilVeinTexture.id)) {
                            ListeSteam[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                        else if (ListeSteam[k].energy_q == 0 && (conv->textureToMove.id == coalTexture.id  || conv->textureToMove.id == oilVeinTexture.id)) {
                            ListeSteam[k].energy_id= conv->textureToMove.id;
                            ListeSteam[k].energy_q++;
                            grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; 
                            conv->textureToMove = (Texture2D){ 0 };
                        } 
                }
            }
    }
}

void Update_Foreuse() {
    float currentTime = GetTime();
    if (currentTime - lastForeuseTime >= 10.0f) {
        for (int i = 0; i < numForeuses; i++) {
            if (ListeForeuse[i].placed && IndexIsValid(ListeForeuse[i].i, ListeForeuse[i].j) && IsEnergieNear(ListeForeuse[i].i, ListeForeuse[i].j,1)) {
                Texture2D texture = grid[ListeForeuse[i].i][ListeForeuse[i].j].texture;
                if (texture.id == copperVeinTexture.id && ListeForeuse[i].q < 100) {
                    ListeForeuse[i].q += 1;
                }
                else if (texture.id == ironVeinTexture.id && ListeForeuse[i].q < 100) {
                    ListeForeuse[i].q += 1;
                }
                else if (texture.id == coalVeinTexture.id && ListeForeuse[i].q < 100) {
                    ListeForeuse[i].q += 1;
                }
                else if (texture.id == waterVeinTexture.id && ListeForeuse[i].q < 100) {
                    ListeForeuse[i].q += 1;
                }
                else if (texture.id == oilVeinTexture.id && ListeForeuse[i].q < 100) {
                    ListeForeuse[i].q += 1;
                }
                printf("Foreuse (%d, %d) mise à jour.\n", ListeForeuse[i].i, ListeForeuse[i].j);
            }
        }

        lastForeuseTime = currentTime;
    }
}


bool isForeuse(int posX, int posY) {
    for (int i = 0; i < numForeuses; i++) {
        if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) {
            return true;
        }
    }
    return false;
}

bool isFurnace(int posX, int posY) {
    for (int i = 0; i < numFurnaces; i++) {
        if (ListeFurnace[i].i == posX && ListeFurnace[i].j == posY) {
            return true;
        }
    }
    return false;
}

bool isHydraulic(int posX, int posY) {
    for (int i = 0; i < numHydraulics; i++) {
        if (ListeHydraulic[i].i == posX && ListeHydraulic[i].j == posY) {
            return true;
        }
    }
    return false;
}

bool isEttireuse(int posX, int posY) {
    for (int i = 0; i < numEttireuses; i++) {
        if (ListeEttireuse[i].i == posX && ListeEttireuse[i].j == posY) {
            return true;
        }
    }
    return false;
}


bool isSteam(int posX, int posY) {
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == posX && ListeSteam[i].j == posY) {
            return true;
        }
    }
    return false;
}

bool isOil(int posX, int posY) {
    for (int i = 0; i < numOils; i++) {
        if (ListeOil[i].i == posX && ListeOil[i].j == posY) {
            return true;
        }
    }
    return false;
}

void RemoveForeuse(int posX, int posY) {
    for (int i = 0; i < numForeuses; i++) {
        if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) {
            for (int j = i; j < numForeuses - 1; j++) {
                ListeForeuse[j] = ListeForeuse[j + 1];
            }
            numForeuses--;
            return;
        }
    }
}

void RemoveFurnace(int posX, int posY) {
    for (int i = 0; i < numFurnaces; i++) {
        if (ListeFurnace[i].i == posX && ListeFurnace[i].j == posY) {
            for (int j = i; j < numFurnaces - 1; j++) {
                ListeFurnace[j] = ListeFurnace[j + 1];
            }
            numFurnaces--;
            return;
        }
    }
}

void RemoveHydraulic(int posX, int posY) {
    for (int i = 0; i < numHydraulics; i++) {
        if (ListeHydraulic[i].i == posX && ListeHydraulic[i].j == posY) {
            for (int j = i; j < numHydraulics - 1; j++) {
                ListeHydraulic[j] = ListeHydraulic[j + 1];
            }
            numHydraulics--;
            return;
        }
    }
}

void RemoveEttireuse(int posX, int posY) {
    for (int i = 0; i < numEttireuses; i++) {
        if (ListeEttireuse[i].i == posX && ListeEttireuse[i].j == posY) {
            for (int j = i; j < numEttireuses - 1; j++) {
                ListeEttireuse[j] = ListeEttireuse[j + 1];
            }
            numEttireuses--;
            return;
        }
    }
}


void RemoveSteam(int posX, int posY) {
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == posX && ListeSteam[i].j == posY) {
            for (int j = i; j < numSteams - 1; j++) {
                ListeSteam[j] = ListeSteam[j + 1];
            }
            numSteams--;
            return;
        }
    }
}

void RemoveOil(int posX, int posY) {
    for (int i = 0; i < numOils; i++) {
        if (ListeOil[i].i == posX && ListeOil[i].j == posY) {
            for (int j = i; j < numOils - 1; j++) {
                ListeOil[j] = ListeOil[j + 1];
            }
            numOils--;
            return;
            grid[posX][posY].moveable = true;
        }
    }
}

void RemoveConveyor(int posX, int posY){
    for (int i = 0; i < MAX_CONVEYOR; i++) {
        if (ListeConveyor[i].i == posX && ListeConveyor[i].j == posY) {
            for (int j = i; j < MAX_CONVEYOR - 1; j++) {
                ListeConveyor[j] = ListeConveyor[j + 1];
            }
            grid[posX][posY].moveable = true;
            return;
        }
    }
}

void RemoveBattery(int posX, int posY){
    for (int i = 0; i < MAX_BATTERY; i++) {
        if (ListeBattery[i].i == posX && ListeBattery[i].j == posY) {
            for (int j = i; j < MAX_BATTERY - 1; j++) {
                ListeBattery[j] = ListeBattery[j + 1];
            }
            grid[posX][posY].moveable = true;
            return;
        }
    }
}


void Update_Furnace() {
    float currentTime = GetTime();
    if (currentTime - lastFurnaceTime >= 20.0f) { // Vérifie le délai global
        for (int i = 0; i < numFurnaces; i++) {
            if (ListeFurnace[i].placed && IndexIsValid(ListeFurnace[i].i, ListeFurnace[i].j)) {
                if(IsEnergieNear(ListeFurnace[i].i,ListeFurnace[i].j,1)) ListeFurnace[i].energy_q++ ;// source d'énergie pas loin
                if (ListeFurnace[i].energy_q > 0 && ListeFurnace[i].material_q > 0) {
                    if (ListeFurnace[i].material_id == copperTexture.id) {
                        ListeFurnace[i].energy_q--;         // Consomme une unité d'énergie
                        ListeFurnace[i].material_q--;      // Consomme une unité de matériau
                        ListeFurnace[i].final_q++;         // Produit une unité de lingot
                        ListeFurnace[i].final_id = copperLingotTexture.id;
                        if (ListeFurnace[i].energy_q == 0) {
                            ListeFurnace[i].energy_id = 0;
                        }
                        if (ListeFurnace[i].material_q == 0) {
                            ListeFurnace[i].material_id = 0;
                        }
                        //printf("Four (%d, %d) : production de lingot de cuivre. Quantité : %d\n", ListeFurnace[i].i, ListeFurnace[i].j, ListeFurnace[i].final_q);
                    } else if (ListeFurnace[i].material_id == ironTexture.id) {
                                ListeFurnace[i].energy_q--;         // Consomme une unité d'énergie
                                ListeFurnace[i].material_q--;      // Consomme une unité de matériau
                                ListeFurnace[i].final_q++;         // Produit une unité de lingot
                                ListeFurnace[i].final_id = ironLingotTexture.id;
                                if (ListeFurnace[i].energy_q == 0) {
                                    ListeFurnace[i].energy_id = 0;
                                }
                                if (ListeFurnace[i].material_q == 0) {
                                    ListeFurnace[i].material_id = 0;
                                }
                                //printf("Four (%d, %d) : production de lingot de fer. Quantité : %d\n", ListeFurnace[i].i, ListeFurnace[i].j, ListeFurnace[i].final_q);
                    }
                    //acier
                    else if (ListeFurnace[i].material_id == ironLingotTexture.id && ListeFurnace[i].energy_q> 3 ) {
                                ListeFurnace[i].energy_q-=2;         // Consomme deux unité d'énergie
                                ListeFurnace[i].material_q;      // Consomme une unité de matériau
                                ListeFurnace[i].final_q++;         // Produit une unité de lingot
                                ListeFurnace[i].final_id = ironLingotTexture.id;
                                if (ListeFurnace[i].energy_q == 0) {
                                    ListeFurnace[i].energy_id = 0;
                                }
                                if (ListeFurnace[i].material_q == 0) {
                                    ListeFurnace[i].material_id = 0;
                                }
                                //printf("Four (%d, %d) : production de lingot de fer. Quantité : %d\n", ListeFurnace[i].i, ListeFurnace[i].j, ListeFurnace[i].final_q);
                    }
                } else {
                    //printf("Four (%d, %d) : pas assez de ressources ou d'énergie\n", ListeFurnace[i].i, ListeFurnace[i].j);
                }
            }
            //printf("DEBUG: Four (%d, %d) - energy_q: %d, material_q: %d, energy_id: %d, material_id: %d\n",ListeFurnace[i].i, ListeFurnace[i].j, ListeFurnace[i].energy_q, ListeFurnace[i].material_q,ListeFurnace[i].energy_id, ListeFurnace[i].material_id);
        }
        lastFurnaceTime = currentTime; // Mise à jour du temps pour la prochaine itération
    }
}

void Update_Hydraulic() {
    float currentTime = GetTime();
    if (currentTime - lastHydraulicTime >= 20.0f) { // Vérifie le délai global
        for (int i = 0; i < numHydraulics; i++) {
            if (ListeHydraulic[i].placed && IndexIsValid(ListeHydraulic[i].i, ListeHydraulic[i].j)) {
                if (IsEnergieNear(ListeHydraulic[i].i, ListeHydraulic[i].j, 1)) ListeHydraulic[i].energy_q++; // source d'énergie pas loin
                if (ListeHydraulic[i].energy_q > 0 && ListeHydraulic[i].material_q > 0) {
                    printf("Presse en action\n");
                    if (ListeHydraulic[i].material_id == ironLingotTexture.id) {
                        ListeHydraulic[i].energy_q--;        // Consomme une unité d'énergie
                        ListeHydraulic[i].material_q--;     // Consomme une unité de matériau
                        ListeHydraulic[i].final_q++;        // Produit une unité de lingot
                        ListeHydraulic[i].final_id = ironPlateTexture.id;
                        if (ListeHydraulic[i].energy_q == 0) ListeHydraulic[i].energy_id = 0;
                        if (ListeHydraulic[i].material_q == 0) ListeHydraulic[i].material_id = 0;
                    } else if (ListeHydraulic[i].material_id == copperLingotTexture.id) {
                        ListeHydraulic[i].energy_q--;
                        ListeHydraulic[i].material_q--;
                        ListeHydraulic[i].final_q++;
                        ListeHydraulic[i].final_id = copperPlateTexture.id;
                        if (ListeHydraulic[i].energy_q == 0) ListeHydraulic[i].energy_id = 0;
                        if (ListeHydraulic[i].material_q == 0) ListeHydraulic[i].material_id = 0;
                    }
                }
            }
        }
        lastHydraulicTime = currentTime; // Mise à jour du temps pour la prochaine itération
    }
}

void Update_Ettireuse() {
    float currentTime = GetTime();
    if (currentTime - lastEttireuseTime >= 20.0f) { // Vérifie le délai global
        for (int i = 0; i < numEttireuses; i++) {
            printf("Etireuse trouvé\n");
            if (ListeEttireuse[i].placed && IndexIsValid(ListeEttireuse[i].i, ListeEttireuse[i].j)) {
                if (IsEnergieNear(ListeEttireuse[i].i, ListeEttireuse[i].j, 1)) ListeEttireuse[i].energy_q++; // source d'énergie pas loin
                printf("énergie : %d ",ListeEttireuse[i].energy_q);
                printf("matériaux : %d,quantité : %d \n",ListeEttireuse[i].material_id,ListeEttireuse[i].material_q);
                if (ListeEttireuse[i].energy_q > 0 && ListeEttireuse[i].material_q > 0) {
                    printf("Prod de l'étireuse,id = %d,%d \n",ListeEttireuse[i].material_id,ironLingotTexture.id);
                    if (ListeEttireuse[i].material_id == ironLingotTexture.id) {
                        ListeEttireuse[i].energy_q--;        // Consomme une unité d'énergie
                        ListeEttireuse[i].material_q--;     // Consomme une unité de matériau
                        ListeEttireuse[i].final_q++;        // Produit une unité de lingot
                        ListeEttireuse[i].final_id = ironRodTexture.id;
                        printf("Production barre fer \n");
                        if (ListeEttireuse[i].energy_q == 0) ListeEttireuse[i].energy_id = 0;
                        if (ListeEttireuse[i].material_q == 0) ListeEttireuse[i].material_id = 0;
                    } else if (ListeEttireuse[i].material_id == copperLingotTexture.id) {
                        ListeEttireuse[i].energy_q--;
                        ListeEttireuse[i].material_q--;
                        ListeEttireuse[i].final_q++;
                        ListeEttireuse[i].final_id = copperRodTexture.id;
                        printf("Production barre cuivre \n");

                        if (ListeEttireuse[i].energy_q == 0) ListeEttireuse[i].energy_id = 0;
                        if (ListeEttireuse[i].material_q == 0) ListeEttireuse[i].material_id = 0;
                    }
                }
            }
        }
        lastEttireuseTime = currentTime; // Mise à jour du temps pour la prochaine itération
    }
}

void Update_Steam() {
    float currentTime = GetTime();
    if (currentTime - lastSteamTime >= 40.0f) {
        for (int i = 0; i < numSteams; i++) {
            if (ListeSteam[i].placed && IndexIsValid(ListeSteam[i].i, ListeSteam[i].j)) {
                if (ListeSteam[i].energy_q > 0 && ListeSteam[i].material_q > 0) {
                    if (ListeSteam[i].material_id == waterVeinTexture.id) {
                        if (ListeSteam[i].final_q < 20) {
                            if (ListeSteam[i].energy_id==oilVeinTexture.id){
                                ListeSteam[i].energy_q--;         // Consomme une unité d'énergie
                                ListeSteam[i].material_q--;      // Consomme une unité d'eau
                                ListeSteam[i].final_q += 6;
                                } 
                            else if (ListeSteam[i].energy_id==coalTexture.id){
                                ListeSteam[i].energy_q--;         // Consomme une unité d'énergie
                                ListeSteam[i].material_q--;      // Consomme une unité d'eau
                                ListeSteam[i].final_q += 4;
                                }    // Produit de l'énergie
                            //printf("Steam (%d, %d) : production. Énergie produite : %d\n", ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].final_q);
                        } else {
                            //printf("Steam (%d, %d) : réservoir plein. Énergie stockée : %d\n", ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].final_q);
                        }
                        if (ListeSteam[i].energy_q == 0) {
                            ListeSteam[i].energy_id = 0;
                        }
                        if (ListeSteam[i].material_q == 0) {
                            ListeSteam[i].material_id = 0;
                        }
                    }
                } else {
                    //printf("Steam (%d, %d) : pas assez de ressources ou d'énergie\n", ListeSteam[i].i, ListeSteam[i].j);
                }
            }
            //printf("DEBUG: Steam (%d, %d) - energy_q: %d, material_q: %d, energy_id: %d, material_id: %d\n",ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].energy_q, ListeSteam[i].material_q,ListeSteam[i].energy_id, ListeSteam[i].material_id);
        }
        lastSteamTime = currentTime;
    }
}

void Update_Oil() {
    float currentTime = GetTime();
    if (currentTime - lastOilTime >= 40.0f) {
        for (int i = 0; i < numOils; i++) {
            if (ListeOil[i].placed && IndexIsValid(ListeOil[i].i, ListeOil[i].j)) {
                if (ListeOil[i].energy_q > 0 && ListeOil[i].material_q > 0) {
                    if (ListeOil[i].material_id == waterVeinTexture.id) {
                        if (ListeSteam[i].final_q < 50) {
                            ListeSteam[i].energy_q--;         // Consomme une unité d'énergie
                            ListeSteam[i].material_q--;      // Consomme une unité d'eau
                            ListeSteam[i].final_q += 8;      // Produit de l'énergie
                            //printf("Steam (%d, %d) : production. Énergie produite : %d\n", ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].final_q);
                        } else {
                            //printf("Steam (%d, %d) : réservoir plein. Énergie stockée : %d\n", ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].final_q);
                        }
                        if (ListeOil[i].energy_q == 0) {
                            ListeOil[i].energy_id = 0;
                        }
                        if (ListeOil[i].material_q == 0) {
                            ListeOil[i].material_id = 0;
                        }
                    }
                } else {
                    //printf("Steam (%d, %d) : pas assez de ressources ou d'énergie\n", ListeSteam[i].i, ListeSteam[i].j);
                }
            }
            //printf("DEBUG: Steam (%d, %d) - energy_q: %d, material_q: %d, energy_id: %d, material_id: %d\n",ListeSteam[i].i, ListeSteam[i].j, ListeSteam[i].energy_q, ListeSteam[i].material_q,ListeSteam[i].energy_id, ListeSteam[i].material_id);
        }
        lastOilTime = currentTime;
    }
}


int AddInInvent(int q, Texture2D texture) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (texture.id == inventory[i].texture.id) {
            inventory[i].quantity += q;
            //printf("objet add\n");
            return 0;
        }
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i].quantity == 0) {
            inventory[i].quantity = q;
            inventory[i].texture = texture;

            const char* name = FindName(texture);
            printf("Nom associé : %s",name);
            if (name != NULL) {
                strncpy(inventory[i].name, name, sizeof(inventory[i].name) - 1);
                inventory[i].name[sizeof(inventory[i].name) - 1] = '\0'; // Ensure null-termination
            } else {
                inventory[i].name[0] = '\0'; // Handle missing name
            }
            return 0; // Success
        }
    }
    return -1;
}

const char* FindName(Texture2D textureRef)
{
    for (int k=0;k< MaxBaseCraft;k++){
        if(BaseCraftInvent[k].texture.id==textureRef.id) return BaseCraftInvent[k].name;
        if(StructCraftInvent[k].texture.id==textureRef.id) return StructCraftInvent[k].name;
        if(MecaCraftInvent[k].texture.id==textureRef.id) return MecaCraftInvent[k].name;
        if(ProdCraftInvent[k].texture.id==textureRef.id) return ProdCraftInvent[k].name;

    }

    return " ";
}


void interraction(int posX, int posY) {
    if (isForeuse(posX, posY)) {
        // Interaction avec une foreuse
        for (int i = 0; i < numForeuses; i++) {
            if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) {
                Texture2D texture = grid[ListeForeuse[i].i][ListeForeuse[i].j].texture;
                if (texture.id == copperVeinTexture.id) {
                    AddInInvent(ListeForeuse[i].q, copperTexture);
                    ListeForeuse[i].q = 0;
                } else if (texture.id == ironVeinTexture.id) {
                    AddInInvent(ListeForeuse[i].q, ironTexture);
                    ListeForeuse[i].q = 0;
                } else if (texture.id == coalVeinTexture.id) {
                    AddInInvent(ListeForeuse[i].q, coalTexture);
                    ListeForeuse[i].q = 0;
                }
            }
        }
    } else if (isHydraulic(posX, posY)) {
        // Interaction avec un hydraulique
        for (int i = 0; i < numHydraulics; i++) {
            if (ListeHydraulic[i].i == posX && ListeHydraulic[i].j == posY) {
                // Ajout de charbon dans l'hydraulique
                if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == coalTexture.id) {
                    inventory[selectedItem].quantity--;
                    ListeHydraulic[i].energy_q++;
                    ListeHydraulic[i].energy_id = coalTexture.id;
                }
                // Ajout de copper dans l'hydraulique
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == copperTexture.id) {
                    if (ListeHydraulic[i].material_id == 0 || ListeHydraulic[i].material_id == copperTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeHydraulic[i].material_q++;
                        ListeHydraulic[i].material_id = copperTexture.id;
                    }
                }
                // Ajout de iron dans l'hydraulique
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == ironTexture.id) {
                    if (ListeHydraulic[i].material_id == 0 || ListeHydraulic[i].material_id == ironTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeHydraulic[i].material_q++;
                        ListeHydraulic[i].material_id = ironTexture.id;
                    }
                }
                // Récupération des produits finis (lingots de cuivre)
                else if (ListeHydraulic[i].final_id == copperLingotTexture.id) {
                    AddInInvent(ListeHydraulic[i].final_q, copperLingotTexture);
                    ListeHydraulic[i].final_q = 0;
                    ListeHydraulic[i].final_id = 0;
                }
                // Récupération des produits finis (lingots de fer)
                else if (ListeHydraulic[i].final_id == ironLingotTexture.id) {
                    AddInInvent(ListeHydraulic[i].final_q, ironLingotTexture);
                    ListeHydraulic[i].final_q = 0;
                    ListeHydraulic[i].final_id = 0;
                }
            }
        }

    } else if (isEttireuse(posX, posY)) {
        // Interaction avec une étireuse
        for (int i = 0; i < numEttireuses; i++) {
            if (ListeEttireuse[i].i == posX && ListeEttireuse[i].j == posY) {
                // Ajout de charbon dans l'étireuse
                if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == coalTexture.id) {
                    inventory[selectedItem].quantity--;
                    ListeEttireuse[i].energy_q++;
                    ListeEttireuse[i].energy_id = coalTexture.id;
                }
                // Ajout de copper dans l'étireuse
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == copperTexture.id) {
                    if (ListeEttireuse[i].material_id == 0 || ListeEttireuse[i].material_id == copperTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeEttireuse[i].material_q++;
                        ListeEttireuse[i].material_id = copperTexture.id;
                    }
                }
                // Ajout de iron dans l'étireuse
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == ironTexture.id) {
                    if (ListeEttireuse[i].material_id == 0 || ListeEttireuse[i].material_id == ironTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeEttireuse[i].material_q++;
                        ListeEttireuse[i].material_id = ironTexture.id;
                    }
                }
                // Récupération des produits finis (lingots de cuivre)
                else if (ListeEttireuse[i].final_id == copperLingotTexture.id) {
                    AddInInvent(ListeEttireuse[i].final_q, copperLingotTexture);
                    ListeEttireuse[i].final_q = 0;
                    ListeEttireuse[i].final_id = 0;
                }
                // Récupération des produits finis (lingots de fer)
                else if (ListeEttireuse[i].final_id == ironLingotTexture.id) {
                    AddInInvent(ListeEttireuse[i].final_q, ironLingotTexture);
                    ListeEttireuse[i].final_q = 0;
                    ListeEttireuse[i].final_id = 0;
                }
            }
        }
        
    } else if (isFurnace(posX, posY)) {
        // Interaction avec un four
        for (int i = 0; i < numFurnaces; i++) {
            if (ListeFurnace[i].i == posX && ListeFurnace[i].j == posY) {
                // Ajout de charbon dans le four
                if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == coalTexture.id) {
                    inventory[selectedItem].quantity--;
                    ListeFurnace[i].energy_q++;
                    ListeFurnace[i].energy_id = coalTexture.id;
                }
                // Ajout de copper dans le four
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == copperTexture.id) {
                    if (ListeFurnace[i].material_id == 0 || ListeFurnace[i].material_id == copperTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeFurnace[i].material_q++;
                        ListeFurnace[i].material_id = copperTexture.id;
                    }
                }
                // Ajout de iron dans le four
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == ironTexture.id) {
                    if (ListeFurnace[i].material_id == 0 || ListeFurnace[i].material_id == ironTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeFurnace[i].material_q++;
                        ListeFurnace[i].material_id = ironTexture.id;
                    }
                }
                // Récupération des produits finis (lingots de cuivre)
                else if (ListeFurnace[i].final_id == copperLingotTexture.id) {
                    AddInInvent(ListeFurnace[i].final_q, copperLingotTexture);
                    ListeFurnace[i].final_q = 0;
                    ListeFurnace[i].final_id = 0;
                }
                // Récupération des produits finis (lingots de fer)
                else if (ListeFurnace[i].final_id == ironLingotTexture.id) {
                    AddInInvent(ListeFurnace[i].final_q, ironLingotTexture);
                    ListeFurnace[i].final_q = 0;
                    ListeFurnace[i].final_id = 0;
                }
            }
        }
    } else if (isSteam(posX, posY)) {
        // Interaction avec un générateur de vapeur
        for (int i = 0; i < numSteams; i++) {
            if (ListeSteam[i].i == posX && ListeSteam[i].j == posY) {
                // Ajout de charbon dans le générateur
                if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == coalTexture.id) {
                    inventory[selectedItem].quantity--;
                    ListeSteam[i].energy_q++;
                    ListeSteam[i].energy_id = coalTexture.id;
                }
                // Ajout de l'eau dans le générateur
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == waterVeinTexture.id) {
                    if (ListeSteam[i].material_id == 0 || ListeSteam[i].material_id == waterVeinTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeSteam[i].material_q++;
                        ListeSteam[i].material_id = waterVeinTexture.id;
                    }
                }
            }
        }
    } else if (isOil(posX, posY)) {
        // Interaction avec un générateur de vapeur
        for (int i = 0; i < numOils; i++) {
            if (ListeOil[i].i == posX && ListeOil[i].j == posY) {
                // Ajout de charbon dans le générateur
                if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == oilVeinTexture.id) {
                    inventory[selectedItem].quantity--;
                    ListeSteam[i].energy_q++;
                    ListeSteam[i].energy_id = oilVeinTexture.id;
                }
                // Ajout de l'eau dans le générateur
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == waterVeinTexture.id) {
                    if (ListeSteam[i].material_id == 0 || ListeSteam[i].material_id == waterVeinTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeSteam[i].material_q++;
                        ListeSteam[i].material_id = waterVeinTexture.id;
                    }
                }
            }
        }
    }
}



int IsEnergieNear(int x, int y,int range) {
    for (int i = -1*range; i <= 1*range; i++) {
        for (int j = -1*range; j <= 1*range; j++) {
            int nx = x + i;
            int ny = y + j;          
            if (IndexIsValid(nx, ny) ) {
                // Vérifie si la texture correspond à un panneau solaire
                if (grid[nx][ny].up_texture.id == solarpanelTexture.id ) return 1;// source elec                
                
                else if (grid[nx][ny].up_texture.id == piloneTexture.id && grid[nx][ny].move_texture.id == piloneEffectTexture.id) {
                    // Vérifie si la cellule source (x, y) est elle-même alimentée
                    if (grid[x][y].move_texture.id == piloneEffectTexture.id) {
                        // NE SUPPRIME PAS la move_texture de nx, ny si elle est déjà alimentée
                        return 1; // Pas besoin de faire d'autres modifications
                    }

                    // Si la cellule source n'est pas alimentée, supprimer l'effet de la cellule cible
                    grid[nx][ny].move_texture = (Texture2D){0};
                    return 1;
                }

                else if(FindNearestSteam(nx, ny)) return 1;
                else if(FindNearestBattery(nx,ny)) return 1; //batterie chargée
                
        }
    }
    }
    return 0;
}

int FindNearestBattery(int x, int y){
    //Trouve la battery connecté
    if (IndexIsValid(x-1,y-1) && IndexIsValid(x+1,y+1)){
    for (int k = 0; k < MAX_BATTERY; k++) {
        if ((ListeBattery[k].i > x-1) && (ListeBattery[k].i < x+1) &&
    (ListeBattery[k].j > y-1) && (ListeBattery[k].j < y+1)) {
            if (ListeBattery[k].q>0){
                ListeBattery[k].q--;
                return 1;
            }
            
        }
    }}
    return 0;
}

int FindNearestSteam(int x,int y){
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i == x && ListeSteam[i].j == y &&ListeSteam[i].final_q>0) {
            ListeSteam[i].final_q--;
            return 1;
        }
    }
    return 0;
}
