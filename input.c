#include "cell.h"
#include "input.h"
#include "inventory.h"
#include "camera.h"
#include <stdio.h>
#include <string.h> // Pour strcmp

int MinPlaceableID = 11; // Liste des ID de textures plaçables
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

int directions[4][2] = {
    {1, 0},
    {-1, 0},
    {0, -1},
    {0, 1} 
    };

void mouseDefault() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    DrawRectangle(posX * cellSize, posY * cellSize, cellSize, cellSize, Fade(RED, 0.3f));
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
                //printf("Name: %s\n", inventory[selectedItem].name);
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
    printf("changement de direction, option : %d\n",conveyor_dir);
}

void ActionWithName(char ObjectName[20], int i, int j,int option) {
    if (strcmp(ObjectName, "Tapis") == 0  ) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                grid[i][j].dir[0] = directions[conveyor_dir][0];
                grid[i][j].dir[1] = directions[conveyor_dir][1];
                ListeConveyor[k] = (Conveyor){.i = i, .j = j,.texture=conveyorTexture, .dir = { directions[conveyor_dir][0], directions[conveyor_dir][1] }, .placed = true,.inMouvement = false, .textureToMove=(Texture2D){0} };
                break;
            }
        }
    } 
    else if (strcmp(ObjectName, "Tuyau") == 0  ) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                grid[i][j].dir[0] = directions[conveyor_dir][0];
                grid[i][j].dir[1] = directions[conveyor_dir][1];
                ListeConveyor[k] = (Conveyor){.i = i, .j = j,.texture=pipeTexture, .dir = { directions[conveyor_dir][0], directions[conveyor_dir][1] }, .placed = true,.inMouvement = false, .textureToMove=(Texture2D){0} };
                break;
            }
        }
    }

    else if (strcmp(ObjectName, "Pilone") == 0  ) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                grid[i][j].dir[0] = directions[conveyor_dir][0];
                grid[i][j].dir[1] = directions[conveyor_dir][1];
                ListeConveyor[k] = (Conveyor){.i = i, .j = j,.texture=piloneTexture, .dir = { directions[conveyor_dir][0], directions[conveyor_dir][1] }, .placed = true,.inMouvement = false, .textureToMove=(Texture2D){0} };
                break;
            }
        }
    }
    else if (strcmp(ObjectName, "Foreuse") == 0) {
        if (numForeuses < MAX_FOREUSE) {
            ListeForeuse[numForeuses++] = (Foreuse){.i = i, .j = j, .q = 0, .placed = true};
            grid[i][j].moveable = false;
        }

    } else if (strcmp(ObjectName, "Furnace") == 0) {
        if (numForeuses < MAX_FURNACE) {
            ListeFurnace[numFurnaces++] = (Furnace){.i = i, .j = j, .energy_q = 0, .energy_id = 0, .material_id = 0, .material_q = 0, .final_id = 0, .final_q = 0, .placed = true};
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
                    ListeConveyor[k].textureToMove = coalTexture;
                    ListeConveyor[k].placed=true;
                    grid[ListeConveyor[k].i][ListeConveyor[k].j].move_texture= ListeConveyor[k].textureToMove;
                }
                
            }
            else Convey(&ListeConveyor[k]);
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
                        if (under_texture.id == copperVeinTexture.id) {
                            mined_texture = copperTexture;
                        }
                        else if (under_texture.id == ironVeinTexture.id) {
                            mined_texture = ironTexture;
                        }
                        else if (under_texture.id == coalVeinTexture.id) {
                            mined_texture = coalTexture;
                        } 
                        conv->textureToMove = mined_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeForeuse[k].q--;
                    }
                    
                }
            }
    }
    else if(grid[srcI][srcJ].up_texture.id == furnaceTexture.id) {
        //Trouve le four connecté
            for (int k = 0; k < numForeuses; k++) {
                if (ListeFurnace[k].i == srcI && ListeFurnace[k].j == srcJ) {
                    if (ListeFurnace[k].final_q>0){
                        int crafted_textureId = ListeFurnace[k].final_id;
                        Texture2D crafted_texture =(Texture2D){0};
                        if (crafted_textureId == copperLingotTexture.id) {
                            crafted_texture = copperLingotTexture;
                        }
                        else if (crafted_textureId == copperLingotTexture.id) {
                            crafted_texture = copperLingotTexture;
                        }
                       
                        conv->textureToMove = crafted_texture;
                        grid[conv->i][conv->j].move_texture=conv->textureToMove;
                        ListeFurnace[k].final_q--;
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

    if (grid[destI][destJ].up_texture.id !=conv->texture.id && !grid[destI][destJ].placed && grid[conv->i][conv->j].move_texture.id!=0) {
        // Déposer l'objet au sol
        grid[destI][destJ].placed = true;
        grid[destI][destJ].up_texture = conv->textureToMove;
        grid[conv->i][conv->j].move_texture = (Texture2D){ 0 }; // Réinitialiser move_texture

        // Réinitialiser l'objet et l'état du convoyeur
        conv->textureToMove = (Texture2D){ 0 };
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
                //printf("Foreuse (%d, %d) mise à jour.\n", ListeForeuse[i].i, ListeForeuse[i].j);
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

void RemoveConveyor(int posX, int posY){
    for (int i = 0; i < MAX_CONVEYOR; i++) {
        if (ListeConveyor[i].i == posX && ListeConveyor[i].j == posY) {
            for (int j = i; j < MAX_CONVEYOR - 1; j++) {
                ListeConveyor[j] = ListeConveyor[j + 1];
            }
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
                } else {
                    //printf("Four (%d, %d) : pas assez de ressources ou d'énergie\n", ListeFurnace[i].i, ListeFurnace[i].j);
                }
            }
            //printf("DEBUG: Four (%d, %d) - energy_q: %d, material_q: %d, energy_id: %d, material_id: %d\n",ListeFurnace[i].i, ListeFurnace[i].j, ListeFurnace[i].energy_q, ListeFurnace[i].material_q,ListeFurnace[i].energy_id, ListeFurnace[i].material_id);
        }
        lastFurnaceTime = currentTime; // Mise à jour du temps pour la prochaine itération
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
            inventory[i].texture =texture;
            return 0;
        }
    }
    return -1;
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
    } else if (isFurnace(posX, posY)) {
        // Interaction avec un four
        for (int i = 0; i < numFurnaces; i++) {
            if (ListeFurnace[i].i == posX && ListeFurnace[i].j == posY) {
                // Ajout de charbon dans le four
                if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == coalTexture.id) {
                    inventory[selectedItem].quantity--;
                    ListeFurnace[i].energy_q++;
                    ListeFurnace[i].energy_id = coalTexture.id;
                    //printf("Charbon: %d\n", ListeFurnace[i].energy_q);
                }
                // Ajout de copper dans le four
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == copperTexture.id) {
                    if (ListeFurnace[i].material_id == 0 || ListeFurnace[i].material_id == copperTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeFurnace[i].material_q++;
                        ListeFurnace[i].material_id = copperTexture.id;
                        //printf("Copper: %d\n", ListeFurnace[i].material_q);
                    } else {
                        //printf("Four occupé\n");
                    }
                }
                // Ajout de iron dans le four
                else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == ironTexture.id) {
                    if (ListeFurnace[i].material_id == 0 || ListeFurnace[i].material_id == ironTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeFurnace[i].material_q++;
                        ListeFurnace[i].material_id = ironTexture.id;
                        //printf("Iron: %d\n", ListeFurnace[i].material_q);
                    } else {
                        //printf("Four occupé\n");
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
    }
}

int IsEnergieNear(int x, int y,int range) {
    for (int i = -1*range; i <= 1*range; i++) {
        for (int j = -1*range; j <= 1*range; j++) {
            int nx = x + i;
            int ny = y + j;          
            if (IndexIsValid(nx, ny) ) {
                // Vérifie si la texture correspond à un panneau solaire
                if (grid[nx][ny].up_texture.id == solarpanelTexture.id ) {
                    return 1;
                }
                else if(grid[nx][ny].up_texture.id==piloneTexture.id &&grid[nx][ny].move_texture.id !=0 ) {
                    grid[nx][ny].move_texture=(Texture2D) {0};
                    return 1;
            }
        }
    }
    }
    return 0;
}
