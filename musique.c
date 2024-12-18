
#include "global.h"
#include <stdlib.h>  // Pour rand() et srand()
#include <stdio.h>


// Définir la variable index pour suivre la musique en cours
int currentMusicIndex = 0;
bool currentChange = false;
bool MusicPlay=true;
bool MusicLoop=false;
bool MusicShuffle=false;
bool shouldClose=false;


void DrawEscapePage(){
    DrawRectangle((screenWidth/2)-400, 10, 800, 60, DARKGRAY);
    DrawText(TextFormat("OPTIONS"), (screenWidth/2)-390, 11, 60, WHITE); // Display quantity
    DrawMusic();
    DrawMusicMenu();
}
// Définir la musique actuellement jouée
Music currentMusic;

// Définir les chemins des fichiers musicaux
const char* musicFiles[NUM_MUSIC_FILES] = {
    "musiques/01.mp3",
    "musiques/02.mp3",
    "musiques/03.mp3",
    "musiques/04.mp3",
    "musiques/05.mp3",
    "musiques/06.mp3",
    "musiques/07.mp3",
    "musiques/08.mp3"
};

void InitMusic() {
    // Initialiser le système audio
    chdir("/Users/charliloumaes/Desktop/Esme/Cours/ingé1/innovation/Jeu-Innovation-ley/Jeu-Innovation");

    InitAudioDevice();
    
    // Charger la première musique
    currentMusic = LoadMusicStream(musicFiles[currentMusicIndex]);
    PlayMusicStream(currentMusic);
}

void UpdateMusic() {
    // Ne pas passer à la musique suivante si la lecture est en pause
    if (MusicPlay) {
        // Mettre à jour le flux audio (si la musique est en lecture)
        UpdateMusicStream(currentMusic);

    }

    if (GetMusicTimeLength(currentMusic)<=GetMusicTimePlayed(currentMusic)+1  && !MusicLoop){ //passe à la suivante si la musique est terminée
        if (MusicShuffle){
                    RandomInt();
                }
                else currentMusicIndex++;
        if (currentMusicIndex >= NUM_MUSIC_FILES) {
            currentMusicIndex = 0;  // Revenir à la première musique
        }
        currentChange=true;
    }
    // Gérer le changement manuel de musique
    if (currentChange) {
        // Décharger la musique actuelle
        UnloadMusicStream(currentMusic);

        // Charger et jouer la nouvelle musique
        currentMusic = LoadMusicStream(musicFiles[currentMusicIndex]);
        PlayMusicStream(currentMusic);
        
        // Réinitialiser le changement de musique
        currentChange = false;
    }
}

void UnloadMusic() {
    // Décharger la musique
    UnloadMusicStream(currentMusic);
    CloseAudioDevice(); // Fermer le système audio si c'est la dernière utilisation
}


void DrawMusic(){
    DrawRectangle((screenWidth/2)-400, 80, 800, 60, DARKGRAY);
    DrawText(TextFormat("Musique : %s",&musicFiles[currentMusicIndex][9]), (screenWidth/2)-390, 81, 60, WHITE);
}
Rectangle MusicPlayButton,MusicPreviewButton,MusicNextButton,MusicShuffleButton,MusicLoopButton,CloseButton,SaveButton,LoadButton;

// Fonction pour initialiser le bouton Play
void MusicButton() {
    MusicLoopButton = (Rectangle) {screenWidth/2-350, 150, 100, 50 };
    MusicPreviewButton = (Rectangle) {screenWidth/2-200, 150, 100, 50 };
    MusicPlayButton = (Rectangle) {(screenWidth/2)-50, 150, 100, 50 };
    MusicNextButton = (Rectangle) {(screenWidth/2)+100, 150, 100, 50 };
    MusicShuffleButton = (Rectangle) {(screenWidth/2)+250, 150, 100, 50 };
    CloseButton=(Rectangle) {(screenWidth/2)-100, 560, 200, 50 };
    SaveButton=(Rectangle) {screenWidth/2-300, 560, 100, 50 };
    LoadButton=(Rectangle) {(screenWidth/2)+200, 560, 100, 50 };

}
void DrawMusicMenu() {
    DrawStat();
    MusicButton();

    // Dessiner les bouttons
if (CheckCollisionPointRec(GetMousePosition(), MusicLoopButton)) {
    DrawRectangleRec(MusicLoopButton, LIGHTGRAY); // Couleur survolée
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        MusicLoop = !MusicLoop;  
    }
}
    else {
        DrawRectangleRec(MusicLoopButton, GRAY);}


if (CheckCollisionPointRec(GetMousePosition(), MusicShuffleButton)) {
    DrawRectangleRec(MusicShuffleButton, LIGHTGRAY); // Couleur survolée
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        MusicShuffle = !MusicShuffle;  
    }
}
    else {
        DrawRectangleRec(MusicShuffleButton, GRAY);}

if (CheckCollisionPointRec(GetMousePosition(), MusicPlayButton)) {
    DrawRectangleRec(MusicPlayButton, LIGHTGRAY); // Couleur survolée
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        MusicPlay = !MusicPlay;  // Basculer entre lecture et pause
        if (MusicPlay) {
            ResumeMusicStream(currentMusic);  // Reprendre la musique
        } else {
            PauseMusicStream(currentMusic);   // Mettre en pause la musique
        }
    }
}
    else {
        DrawRectangleRec(MusicPlayButton, GRAY);}

   if (CheckCollisionPointRec(GetMousePosition(),MusicPreviewButton )) {
        DrawRectangleRec(MusicPreviewButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            if (currentMusicIndex>0){
                if (MusicShuffle){
                    RandomInt();
                }
                else currentMusicIndex--;
                
                currentChange=true;
            }
            else {
                currentMusicIndex=NUM_MUSIC_FILES-1;
                currentChange=true;
            }
        }
        } 
    else {
        DrawRectangleRec(MusicPreviewButton, GRAY);  // Couleur normale

    }

    if (CheckCollisionPointRec(GetMousePosition(), MusicNextButton)) {
        DrawRectangleRec(MusicNextButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            if (currentMusicIndex<NUM_MUSIC_FILES-1){
                if (MusicShuffle){
                    RandomInt();
                }
                else currentMusicIndex++;

                currentChange=true;
            }
            else 
            {
                currentMusicIndex=0;
                currentChange=true;

            }

        }
        } 
    else {
        DrawRectangleRec(MusicNextButton, GRAY);  // Couleur normale

    }
    if(MusicPlay) DrawText(TextFormat("PAUSE /\nPLAY"), MusicPlayButton.x+10, 150, 20, WHITE);
    else DrawText(TextFormat("PAUSE /\nPLAY"), MusicPlayButton.x+10, 150, 20, BLUE);

    DrawText(TextFormat("Preview"), MusicPreviewButton.x+10, 150, 20, WHITE);
    DrawText(TextFormat("Next "), MusicNextButton.x+10, 150, 20, WHITE);

    if(MusicShuffle) DrawText(TextFormat("Shuffle "), MusicShuffleButton.x+10, 150, 20, BLUE);
    else DrawText(TextFormat("Shuffle "), MusicShuffleButton.x+10, 150, 20, WHITE);

    if (MusicLoop) DrawText(TextFormat("Loop "), MusicLoopButton.x+10, 150, 20, BLUE);
    else DrawText(TextFormat("Loop "), MusicLoopButton.x+10, 150, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(),CloseButton )) {
        DrawRectangleRec(CloseButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            shouldClose=true;
            }
        
        } 
    else {
        DrawRectangleRec(CloseButton, GRAY);  // Couleur normale
    }
    DrawText(TextFormat("QUIT-SAVE BEFORE"), CloseButton.x + 10, CloseButton.y + (CloseButton.height / 2) - 10, 19, WHITE);


    if (CheckCollisionPointRec(GetMousePosition(),LoadButton )) {
        DrawRectangleRec(LoadButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            load();
            }
        
        } 
    else {
        DrawRectangleRec(LoadButton, GRAY);  // Couleur normale
    }
    DrawText(TextFormat("LOAD"), LoadButton.x + 10, LoadButton.y + (LoadButton.height / 2) - 10, 19, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(),SaveButton )) {
        DrawRectangleRec(SaveButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            save();
            }
        
        } 
    else {
        DrawRectangleRec(SaveButton, GRAY);  // Couleur normale
    }
    DrawText(TextFormat("SAVE"), SaveButton.x + 10, SaveButton.y + (SaveButton.height / 2) - 10, 19, WHITE);
}

void RandomInt() {
    int old = currentMusicIndex;  // Sauvegarde de la valeur actuelle
    do {
        currentMusicIndex = rand() % NUM_MUSIC_FILES;  // Génération d'un nouvel index
    } while (currentMusicIndex == old);  // Comparer les valeurs pour éviter la répétition
}

void DrawStat(){
    DrawRectangle((screenWidth/2)-400, 250, 800, 60, DARKGRAY);
    DrawText(TextFormat("Satistiques"), (screenWidth/2)-390, 250, 60, WHITE);
    DrawText(TextFormat("Craft : %d",craftedObjects), (screenWidth/2)-390, 350, 50, LIGHTGRAY);
    DrawText(TextFormat("Objets ramassé : %d",pickedObject), (screenWidth/2)-390, 420, 50, LIGHTGRAY);
    DrawText(TextFormat("Temps écoulé : %d",days), (screenWidth/2)-390, 490, 50, LIGHTGRAY);

}

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

