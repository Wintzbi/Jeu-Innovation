#include "cell.h"
#include "input.h"
#include "inventory.h"
#include "camera.h"
#include <stdio.h>
#include <string.h> // Pour strcmp

int MinPlaceableID = 11; // Liste des ID de textures plaçables
Conveyor ListeConveyor[MAX_CONVEYOR];
Texture2D textureToMove = (Texture2D){0};
bool inMouvement = false;
Foreuse ListeForeuse[MAX_FOREUSE];

int numForeuses = 0;
float lastForeuseTime;

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

bool isForeuse(int posX, int posY);

void rightClic() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        if (isForeuse(posX, posY)) {
            printf("Ceci est une foreuse\n");
            for (int i = 0; i < numForeuses; i++) {
                if (ListeForeuse[i].i == posX && ListeForeuse[i].j == posY) {
                    Texture2D texture = grid[ListeForeuse[i].i][ListeForeuse[i].j].texture;
                    if (texture.id == copperVeinTexture.id) {
                        AddInInvent(ListeForeuse[i].q, copperTexture);
                        ListeForeuse[i].q = 0;
                    }
                    else if (texture.id == ironVeinTexture.id) {
                        AddInInvent(ListeForeuse[i].q, ironTexture);
                        ListeForeuse[i].q = 0;
                    }
                    else if (texture.id == coalVeinTexture.id) {
                        AddInInvent(ListeForeuse[i].q, coalTexture);
                        ListeForeuse[i].q = 0;
                    }
                }
            }

        } else if (inventory[selectedItem].quantity > 0 &&
                   inventory[selectedItem].texture.id >= MinPlaceableID) {
            if (IndexIsValid(posX, posY) && !grid[posX][posY].placed) {
                grid[posX][posY].placed = true;
                grid[posX][posY].up_texture = inventory[selectedItem].texture;
                printf("Name: %s\n", inventory[selectedItem].name);
                ActionWithName(inventory[selectedItem].name, posX, posY);
                inventory[selectedItem].quantity--;

                if (inventory[selectedItem].quantity == 0) {
                    inventory[selectedItem].texture = (Texture2D){0};
                }
            }
        }
    }
}

void RemoveForeuse(int posX, int posY);

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
            pickedObject++;

            // Désactiver une foreuse si elle est présente
            for (int f = 0; f < numForeuses; f++) {
                if (ListeForeuse[f].i == posX && ListeForeuse[f].j == posY && ListeForeuse[f].placed) {
                    ListeForeuse[f].placed = false;
                    RemoveForeuse(ListeForeuse[f].i, ListeForeuse[f].j);
                    break;
                }
            }
        }
    }
}

void ActionWithName(char ObjectName[20], int i, int j) {
    if (strcmp(ObjectName, "Tapis") == 0) {
        for (int k = 0; k < MAX_CONVEYOR; k++) {
            if (!ListeConveyor[k].placed) {
                ListeConveyor[k] = (Conveyor){.i = i, .j = j, .dir = {1, 0}, .placed = true};
                break;
            }
        }
    } else if (strcmp(ObjectName, "Foreuse") == 0) {
        if (numForeuses < MAX_FOREUSE) {
            ListeForeuse[numForeuses++] = (Foreuse){.i = i, .j = j, .q = 0, .placed = true};
            grid[i][j].moveable = false;
        }
    }
}

void Update_Conv() {
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (ListeConveyor[k].placed) {
            Convey(ListeConveyor[k]);
        }
    }
}

void Convey(Conveyor conv){
        // on prend l'objet à déplacer qui est au sol
        if(!inMouvement && grid[conv.i - conv.dir[0]][conv.j- conv.dir[1]].moveable && grid[conv.i - conv.dir[0]][conv.j- conv.dir[1]].up_texture.id !=conveyorTexture.id && grid[conv.i - conv.dir[0]][conv.j- conv.dir[1]].up_texture.id !=0){
            textureToMove=grid[conv.i - conv.dir[0]][conv.j- conv.dir[1]].up_texture;
            //on supprime l'objet déplacé
            grid[conv.i - conv.dir[0]][conv.j- conv.dir[1]].placed=false;
            grid[conv.i - conv.dir[0]][conv.j- conv.dir[1]].up_texture = (Texture2D){ 0 };
            inMouvement=true;
        }
        // si il est dans un objet comme la foreuse
        else if(!inMouvement && grid[conv.i - conv.dir[0]][conv.j- conv.dir[1]].up_texture.id == drillTexture.id) {
             Foreuse* NearForeuse = NULL; // Réinitialisation du pointeur avant la recherche

            for (int k = 0; k < MAX_FOREUSE; k++) {
                if (ListeForeuse[k].placed) {
                    if ((conv.i - conv.dir[0]) == ListeForeuse[k].i && (conv.j - conv.dir[1]) == ListeForeuse[k].j) {
                        NearForeuse = &ListeForeuse[k]; // Affectation du pointeur
                        printf("Foreuse identifiée\n");
                        break; // On arrête la recherche une fois la foreuse trouvée
                    }
                }
            }
             printf("pointeur : %d",NearForeuse);
             printf("quanrtité %d\n",NearForeuse->q);
            if (NearForeuse != NULL && NearForeuse->q >0){
               
                printf("Quantité enlevé\n");
                NearForeuse->q -= 1;
                textureToMove= NearForeuse->texture;
                inMouvement=true;
            }
            
        }
        //vérifie le bloc d'apès
        else if (inMouvement && IndexIsValid(conv.i + conv.dir[0], conv.j+ conv.dir[1])  && textureToMove.id !=0){
            //on déplace l'objet dans l'inventaire
            if (grid[conv.i + conv.dir[0]][conv.j+ conv.dir[1]].up_texture.id == chestTexture.id)
            {
                
                AddInInvent(1,textureToMove);
            }
            //sinon au sol
            else if (!grid[conv.i + conv.dir[0]][conv.j+ conv.dir[1]].placed) {
                grid[conv.i + conv.dir[0]][conv.j+ conv.dir[1]].placed = true;
                grid[conv.i + conv.dir[0]][conv.j+ conv.dir[1]].up_texture =textureToMove;             
            }
            //réinitialise le mouvement
            textureToMove=(Texture2D){ 0 } ;
            grid[conv.i][conv.j].move_texture=(Texture2D){ 0 };
            inMouvement=false;
        }
        else if(inMouvement && textureToMove.id !=0 ) {
            grid[conv.i + conv.dir[0]][conv.j+ conv.dir[1]].move_texture =textureToMove;
            grid[conv.i][conv.j].move_texture=(Texture2D){ 0 };
            
        }
    }

void Update_Foreuse() {
    float currentTime = GetTime();
    if (currentTime - lastForeuseTime >= 10.0f) {
        for (int i = 0; i < numForeuses; i++) {
            if (ListeForeuse[i].placed && IndexIsValid(ListeForeuse[i].i, ListeForeuse[i].j)) {
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


int AddInInvent(int q, Texture2D texture) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (texture.id == inventory[i].texture.id) {
            inventory[i].quantity += q;
            printf("objet add\n");
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
}