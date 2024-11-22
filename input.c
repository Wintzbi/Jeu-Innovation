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
Furnace ListeFurnace[MAX_FURNACE];

int numForeuses = 0;
float lastForeuseTime;

int numFurnaces = 0;
float lastFurnaceTime;

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
bool isFurnace(int posX, int poxY);

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

        } else if (isFurnace(posX, posY)) {
            printf("Ceci est un four\n");
            for (int i = 0; i < numFurnaces; i++) {
                if (ListeFurnace[i].i == posX && ListeFurnace[i].j == posY) {
                    if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == coalTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeFurnace[i].energy_q++;
                        ListeFurnace[i].energy_id = coalTexture.id;
                        printf("Charbon: %d\n",ListeFurnace[i].energy_q);
                    }
                    else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == copperTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeFurnace[i].material_q++;
                        ListeFurnace[i].material_id = copperTexture.id;
                        printf("Copper: %d\n",ListeFurnace[i].material_q);
                    }
                    else if (inventory[selectedItem].quantity > 0 && inventory[selectedItem].texture.id == ironTexture.id) {
                        inventory[selectedItem].quantity--;
                        ListeFurnace[i].material_q++;
                        ListeFurnace[i].material_id = ironTexture.id;
                        printf("Copper: %d\n",ListeFurnace[i].material_q);
                    }
                    else {
                        AddInInvent(ListeFurnace[i].final_q, copperLingotTexture);
                        ListeFurnace[i].final_q = 0;
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
            pickedObject++;

            // Désactiver une foreuse si elle est présente
            for (int f = 0; f < numForeuses; f++) {
                if (ListeForeuse[f].i == posX && ListeForeuse[f].j == posY && ListeForeuse[f].placed) {
                    ListeForeuse[f].placed = false;
                    RemoveForeuse(ListeForeuse[f].i, ListeForeuse[f].j);
                    break;
                }
            }

            for (int f = 0; f < numFurnaces; f++) {
                if (ListeFurnace[f].i == posX && ListeFurnace[f].j == posY && ListeFurnace[f].placed) {
                    ListeFurnace[f].placed = false;
                    RemoveFurnace(ListeFurnace[f].i, ListeFurnace[f].j);
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
                ListeConveyor[k] = (Conveyor){.i = i, .j = j, .dir = {1, 0}, .placed = true,.inMouvement = false, .textureToMove=(Texture2D){0} };
                break;
            }
        }
    } else if (strcmp(ObjectName, "Foreuse") == 0) {
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
            Convey(&ListeConveyor[k]);
        }
    }
}

void Convey(Conveyor *conv) {
    int srcI = conv->i - conv->dir[0];  // Calcul de la case source
    int srcJ = conv->j - conv->dir[1];  // Calcul de la case source
    int destI = conv->i + conv->dir[0]; // Calcul de la case destination
    int destJ = conv->j + conv->dir[1]; // Calcul de la case destination

    if (!IndexIsValid(srcI, srcJ) || !IndexIsValid(destI, destJ)) {
        printf("Convoyeur (%d, %d) : indices invalides\n", conv->i, conv->j);
        return; // Eviter l'accès aux indices invalides
    }

    // Vérifier si un objet est prêt à être pris (case source)
    if (!conv->inMouvement && 
        grid[srcI][srcJ].moveable && 
        grid[srcI][srcJ].up_texture.id != 0 && 
        grid[srcI][srcJ].up_texture.id != conveyorTexture.id) {
        // Prendre l'objet de la case source
        conv->textureToMove = grid[srcI][srcJ].up_texture;
        grid[srcI][srcJ].placed = false;
        grid[srcI][srcJ].up_texture = (Texture2D){ 0 }; // Effacer la case source
        conv->inMouvement = true;
        printf("Convoyeur (%d, %d) : début de mouvement, texture %d\n", conv->i, conv->j, conv->textureToMove.id);
    }
    // Récupérer l'objet d'un convoyeur précédent
    else if (!conv->inMouvement &&
        grid[srcI][srcJ].move_texture.id != 0 && 
        grid[srcI][srcJ].up_texture.id == conveyorTexture.id) {
        conv->textureToMove = grid[srcI][srcJ].move_texture;
        grid[srcI][srcJ].move_texture = (Texture2D){ 0 }; // Réinitialiser move_texture
        conv->inMouvement = true;
        printf("Convoyeur (%d, %d) : continuité de mouvement, texture %d\n", conv->i, conv->j, conv->textureToMove.id);
    }

    // Si en mouvement, vérifier la destination
    if (conv->inMouvement && conv->textureToMove.id != 0) {
        if (IndexIsValid(destI, destJ)) {
            if (grid[destI][destJ].up_texture.id == conveyorTexture.id) {
                // Déplacer l'objet visuellement sur le convoyeur suivant
                grid[destI][destJ].move_texture = conv->textureToMove;
                grid[srcI][srcJ].move_texture = (Texture2D){ 0 }; // Effacer la case source visuellement
                printf("Convoyeur (%d, %d) : l'objet est sur un convoyeur à (%d, %d)\n", conv->i, conv->j, destI, destJ);
            } else if (!grid[destI][destJ].placed) {
                // Déposer l'objet au sol
                grid[destI][destJ].placed = true;
                grid[destI][destJ].up_texture = conv->textureToMove;
                grid[srcI][srcJ].move_texture = (Texture2D){ 0 }; // Réinitialiser move_texture
                printf("Convoyeur (%d, %d) : objet déplacé à (%d, %d)\n", conv->i, conv->j, destI, destJ);

                // Réinitialiser l'objet et l'état du convoyeur
                conv->textureToMove = (Texture2D){ 0 };
                conv->inMouvement = false;
            } else {
                // La case suivante est occupée par un autre objet
                printf("Convoyeur (%d, %d) : en attente, case (%d, %d) occupée\n", conv->i, conv->j, destI, destJ);
            }
        }
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

void Update_Furnace() {
    float currentTime = GetTime();
    if (currentTime - lastFurnaceTime >= 20.0f) { // Vérifie le délai global
        for (int i = 0; i < numFurnaces; i++) {
            if (ListeFurnace[i].placed && IndexIsValid(ListeFurnace[i].i, ListeFurnace[i].j)) {
                if (ListeFurnace[i].energy_q > 0 && ListeFurnace[i].material_q > 0) {
                    if (ListeFurnace[i].material_id == copperTexture.id) {
                        ListeFurnace[i].energy_q--;         // Consomme une unité d'énergie
                        ListeFurnace[i].material_q--;      // Consomme une unité de matériau
                        ListeFurnace[i].final_q++;         // Produit une unité de lingot
                        ListeFurnace[i].final_id = copperLingotTexture.id;
                        printf("Four (%d, %d) : production de lingot de cuivre. Quantité : %d\n", 
                               ListeFurnace[i].i, ListeFurnace[i].j, ListeFurnace[i].final_q);
                    }
                } else {
                    printf("Four (%d, %d) : pas assez de ressources ou d'énergie\n", 
                           ListeFurnace[i].i, ListeFurnace[i].j);
                }
            }
        }
        lastFurnaceTime = currentTime; // Mise à jour du temps pour la prochaine itération
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