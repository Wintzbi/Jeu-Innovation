#include "cell.h"
#include "input.h"
#include "inventory.h"
#include "camera.h"
#include <stdio.h>
#include <string.h> // Pour strcmp

int MinPlaceableID = 11; // Liste des ID de textures plaçables
Conveyor ListeConveyor[MAX_CONVEYOR];
Foreuse ListeForeuse[MAX_FOREUSE];
int numForeuses = 0;

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

void rightClic() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && inventory[selectedItem].quantity > 0 &&
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
            ListeForeuse[numForeuses++] = (Foreuse){.i = i, .j = j, .placed = true};
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

void Convey(Conveyor conv) {
    if (IndexIsValid(conv.i + conv.dir[0], conv.j + conv.dir[1]) &&
        !grid[conv.i + conv.dir[0]][conv.j + conv.dir[1]].placed) {
        grid[conv.i + conv.dir[0]][conv.j + conv.dir[1]].placed = true;
        grid[conv.i + conv.dir[0]][conv.j + conv.dir[1]].up_texture =
            grid[conv.i][conv.j].up_texture;

        grid[conv.i][conv.j].placed = false;
        grid[conv.i][conv.j].up_texture = (Texture2D){0};
    }
}

void Update_Foreuse() {
    for (int i = 0; i < numForeuses; i++) {
        if (ListeForeuse[i].placed && IndexIsValid(ListeForeuse[i].i, ListeForeuse[i].j)) {
            // Récupérer l'identifiant de la texture sur laquelle se trouve la foreuse
            Texture2D texture = grid[ListeForeuse[i].i][ListeForeuse[i].j].texture;
            printf("Foreuse active sur la texture ID '%d'\n", texture.id);
        }
    }
}
