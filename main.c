#include "global.h"

GameScreen currentScreen = MENU;
bool isInventoryScreenOpen = false;
bool isOptionScreenOpen = false;
bool isCraftScreenOpen=false;
bool isMapScreenOpen=false;
bool isGameInitialized = false;
void CurrentScreenFix(){
    if(currentScreen == GAME)
    {
        isInventoryScreenOpen = false;
        isOptionScreenOpen = false;
        isCraftScreenOpen=false;
        isMapScreenOpen=false;
        }
}
//Save load
int save() {
    // Ouvrir le fichier en mode binaire pour écrire
    FILE *file = fopen("save.dat", "wb");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    }

    // Sauvegarder le nombre de cellules de la grille
    int gridSize = ROW * COL;
    fwrite(&gridSize, sizeof(int), 1, file);  // Sauvegarder la taille de la grille (ROW * COL)

    // Sauvegarder la grille
    fwrite(grid, sizeof(Cell), gridSize, file);  // Sauvegarder toutes les cellules de la grille

    // Sauvegarder l'inventaire
    fwrite(inventory, sizeof(Item), INVENTORY_SIZE, file);  // Sauvegarder l'inventaire

    // Sauvegarder le nombre de foreuses et la liste
    fwrite(&numForeuses, sizeof(int), 1, file);  // Sauvegarder le nombre de foreuses
    fwrite(ListeForeuse, sizeof(Foreuse), numForeuses, file);  // Sauvegarder les foreuses

    // Sauvegarder le nombre de furnaces et la liste
    fwrite(&numFurnaces, sizeof(int), 1, file);  // Sauvegarder le nombre de furnaces
    fwrite(ListeFurnace, sizeof(Furnace), numFurnaces, file);  // Sauvegarder les furnaces

    // Sauvegarder le nombre d'hydraulics et la liste
    fwrite(&numHydraulics, sizeof(int), 1, file);  // Sauvegarder le nombre d'hydraulics
    fwrite(ListeHydraulic, sizeof(Hydraulic), numHydraulics, file);  // Sauvegarder les hydraulics

    // Sauvegarder le nombre d'ettireuses et la liste
    fwrite(&numEttireuses, sizeof(int), 1, file);  // Sauvegarder le nombre d'ettireuses
    fwrite(ListeEttireuse, sizeof(Ettireuse), numEttireuses, file);  // Sauvegarder les ettireuses

    // Sauvegarder le nombre de steams et la liste
    fwrite(&numSteams, sizeof(int), 1, file);  // Sauvegarder le nombre de steams
    fwrite(ListeSteam, sizeof(Steam), numSteams, file);  // Sauvegarder les steams

    // Sauvegarder le nombre d'oils et la liste
    fwrite(&numOils, sizeof(int), 1, file);  // Sauvegarder le nombre d'oils
    fwrite(ListeOil, sizeof(Oil), numOils, file);  // Sauvegarder les oils

    // Sauvegarder la liste des conveyors
    fwrite(ListeConveyor, sizeof(Conveyor), MAX_CONVEYOR, file);  // Sauvegarder les conveyors

    // Sauvegarder la liste des batteries
    fwrite(ListeBattery, sizeof(Battery), MAX_BATTERY, file);  // Sauvegarder les batteries

    fclose(file);
    return 0;
}


int load() {
    // Ouvrir le fichier en mode binaire pour lire
    FILE *file = fopen("save.dat", "rb");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    }

    // Lire la taille de la grille
    int gridSize;
    fread(&gridSize, sizeof(int), 1, file);

    // Lire la grille
    fread(grid, sizeof(Cell), gridSize, file);  // Lire les cellules de la grille

    // Lire l'inventaire
    fread(inventory, sizeof(Item), INVENTORY_SIZE, file);  // Lire l'inventaire

    // Lire le nombre de foreuses et la liste
    fread(&numForeuses, sizeof(int), 1, file);  // Lire le nombre de foreuses
    fread(ListeForeuse, sizeof(Foreuse), numForeuses, file);  // Lire les foreuses

    // Lire le nombre de furnaces et la liste
    fread(&numFurnaces, sizeof(int), 1, file);  // Lire le nombre de furnaces
    fread(ListeFurnace, sizeof(Furnace), numFurnaces, file);  // Lire les furnaces

    // Lire le nombre d'hydraulics et la liste
    fread(&numHydraulics, sizeof(int), 1, file);  // Lire le nombre d'hydraulics
    fread(ListeHydraulic, sizeof(Hydraulic), numHydraulics, file);  // Lire les hydraulics

    // Lire le nombre d'ettireuses et la liste
    fread(&numEttireuses, sizeof(int), 1, file);  // Lire le nombre d'ettireuses
    fread(ListeEttireuse, sizeof(Ettireuse), numEttireuses, file);  // Lire les ettireuses

    // Lire le nombre de steams et la liste
    fread(&numSteams, sizeof(int), 1, file);  // Lire le nombre de steams
    fread(ListeSteam, sizeof(Steam), numSteams, file);  // Lire les steams

    // Lire le nombre d'oils et la liste
    fread(&numOils, sizeof(int), 1, file);  // Lire le nombre d'oils
    fread(ListeOil, sizeof(Oil), numOils, file);  // Lire les oils

    // Lire la liste des conveyors
    fread(ListeConveyor, sizeof(Conveyor), MAX_CONVEYOR, file);  // Lire les conveyors

    // Lire la liste des batteries
    fread(ListeBattery, sizeof(Battery), MAX_BATTERY, file);  // Lire les batteries


    fclose(file);
    return 0;
}

void InitGame() {
    InitTexture();
    InitGrid();
    InitMusic();
    InitInventory();  // Initialiser l'inventaire avec des textures et des quantités d'exemple
    InitBaseCraft();
    setPlayerCamera();
    isGameInitialized = true;  // Marquer le jeu comme initialisé
}

int main(void) {
    update_size();
    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

    ButtonPlay(); // Initialiser le bouton Play

    SetTargetFPS(60);  // Définir la fréquence d'images cible

    const double interval = 1; // Intervalle en secondes
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (!shouldClose) {
        update_size();
        clock_gettime(CLOCK_MONOTONIC, &current);

        double elapsed = (current.tv_sec - start.tv_sec) +
                         (current.tv_nsec - start.tv_nsec) / 1e9;

        if (elapsed >= interval) {
            Update_Conv(); // Mettre à jour les conveyors
            clock_gettime(CLOCK_MONOTONIC, &start);
        }

        if (IsKeyPressed(KEY_R)) UpdateDir();

        UpdateMusic();

        InitInventoryKeyBiding();
        UpdateBattery();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Afficher le menu principal
        if (currentScreen == MENU) {
            DrawMenu(&currentScreen);

            // Initialiser le jeu si le bouton Play est pressé
            if (currentScreen == GAME && !isGameInitialized) {
                InitGame();
            }
        }

        // Gestion des écrans secondaires
        if (IsKeyPressed(KEY_E)) {
            isInventoryScreenOpen = !isInventoryScreenOpen;
            currentScreen = isInventoryScreenOpen ? INVENT : GAME;
        }

        if (IsKeyPressed(KEY_SEMICOLON)) {
            isMapScreenOpen = !isMapScreenOpen;
            currentScreen = isMapScreenOpen ? MAP : GAME;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            isOptionScreenOpen = !isOptionScreenOpen;
            currentScreen = isOptionScreenOpen ? OPTION : GAME;
        }

        if (IsKeyPressed(KEY_C)) {
            isCraftScreenOpen = !isCraftScreenOpen;
            currentScreen = isCraftScreenOpen ? CRAFT : GAME;
        }

        if (IsKeyPressed(KEY_TAB)) {
            selectedItem = (selectedItem + 1) % 10;
        }

        // Dessiner les différents écrans
        if (currentScreen == INVENT) {
            DrawInventoryPage();
        } else if (currentScreen == OPTION) {
            DrawEscapePage();
        } else if (currentScreen == CRAFT) {
            DrawCraftPage();
        } else if (currentScreen == MAP) {
            DrawMap();
        }

        // Mode 2D pour le jeu
        BeginMode2D(camera);

        if (currentScreen == GAME) {
            CurrentScreenFix();
            GridDraw();  // Dessiner la grille de jeu
            rightClic();
            leftClic();
            mouseDefault();
            moveCamera();
            Update_Foreuse();
            Update_Furnace();
            Update_Steam();
            Update_Hydraulic();
            Update_Ettireuse();
            DrawMiniMap();
        }

        EndMode2D();

        // Dessiner la barre d'inventaire
        if (currentScreen == GAME) {
            DrawInventoryBar();
        }

        EndDrawing();
    }

    // Libération des ressources
    UnloadMusic();
    UnloadAllTexture();
    CloseWindow();

    return 0;
}

