#include "global.h"
GameScreen currentScreen = MENU;
bool isInventoryScreenOpen = false;
bool isOptionScreenOpen = false;
bool isCraftScreenOpen=false;
bool isMapScreenOpen=false;
void CurrentScreenFix(){
    if(currentScreen == GAME)
    {
        isInventoryScreenOpen = false;
        isOptionScreenOpen = false;
        isCraftScreenOpen=false;
        isMapScreenOpen=false;
        }
}
int main(void) {

    InitWindow(screenWidth, screenHeight, "Minc Corp simulation with inventory");   // Initialisation de la fenêtre
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires
    InitTexture();
    InitGrid();
    InitMusic();

    InitInventory();  // Initialiser l'inventaire avec des textures et des quantités d'exemple
    InitBaseCraft();
    SetTargetFPS(60);  // Définir la fréquence d'images cible à 60 FPS

    setPlayerCamera();

    

    ButtonPlay();  // Initialiser le bouton Play

    const double interval = 1; // Intervalle en secondes
    struct timespec start, current; 
    clock_gettime(CLOCK_MONOTONIC, &start); // Temps de départ
    while (!shouldClose ) {

        clock_gettime(CLOCK_MONOTONIC, &current); // Temps actuel

        double elapsed = (current.tv_sec - start.tv_sec) +
                         (current.tv_nsec - start.tv_nsec) / 1e9;

        if (elapsed >= interval) {
            // Code à exécuter toutes les demi-secondes
            Update_Conv();
            clock_gettime(CLOCK_MONOTONIC, &start); // Réinitialiser le temps de départ
        }

        if (IsKeyPressed(KEY_R)) UpdateDir();
        UpdateMusic();
        InitInventoryKeyBiding();
        UpdateBattery();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU) {
            DrawMenu(&currentScreen);
            
        }

        if (IsKeyPressed(KEY_E)) {
            isInventoryScreenOpen = !isInventoryScreenOpen;
            if (isInventoryScreenOpen) {
                currentScreen = INVENT;
            }
            else currentScreen = GAME;
        }


        if (IsKeyPressed(KEY_SEMICOLON)) {
                isMapScreenOpen = !isMapScreenOpen;  
                if (isMapScreenOpen) {
                    currentScreen = MAP;  // Revenir à l'état GAME si l'inventaire est fermé
                }
                else currentScreen = GAME;
            }

        if (IsKeyPressed(KEY_ESCAPE)) {
            isOptionScreenOpen = !isOptionScreenOpen;
            if (isOptionScreenOpen) {
                currentScreen = OPTION;
            }
            else currentScreen = GAME;
        }

        if (IsKeyPressed(KEY_C)) {
                isCraftScreenOpen = !isCraftScreenOpen;  // Ouvrir/fermer l'inventaire avec la touche E

                if (isCraftScreenOpen) {
                    currentScreen = CRAFT;  // Revenir à l'état GAME si l'inventaire est fermé
                }
                else currentScreen = GAME;
            }
        if (IsKeyPressed(KEY_TAB)) {
            selectedItem=(selectedItem+1)%10;
        }
        
        if (currentScreen == INVENT) {
            DrawInventoryPage();
        } else if (currentScreen ==OPTION) {
            DrawEscapePage();
        } 

        else if (currentScreen == CRAFT) {
            DrawCraftPage();
        }
        else if (currentScreen == MAP) {
            DrawMap();
        }

        BeginMode2D(camera); 

        if (currentScreen == GAME) {
            CurrentScreenFix();
            GridDraw();  // Dessiner la grille de jeu
            

            rightClic();  // Placer un bloc avec un clic droit

            leftClic();  // Récupérer un bloc avec un clic gauche

            mouseDefault();  // Afficher la souris par défaut

            moveCamera();

            Update_Foreuse();

            Update_Furnace();

            Update_Steam();

            DrawMiniMap();

        }
        EndMode2D();

        if (currentScreen == GAME) {
        DrawInventoryBar();  // Dessiner la barre de l'inventaire
        }
        EndDrawing();
    }

    UnloadMusic();
    UnloadAllTexture();
    CloseWindow();

    return 0;
}
