#include "musique.h"
#include <stdlib.h>  // Pour rand() et srand()


// Définir la variable index pour suivre la musique en cours
int currentMusicIndex = 0;
bool currentChange = false;
bool MusicPlay=true;
bool MusicLoop=false;
bool MusicShuffle=false;


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
Rectangle MusicPlayButton,MusicPreviewButton,MusicNextButton,MusicShuffleButton,MusicLoopButton;

// Fonction pour initialiser le bouton Play
void MusicButton() {
    MusicLoopButton = (Rectangle) {screenWidth/2-350, 150, 100, 50 };
    MusicPreviewButton = (Rectangle) {screenWidth/2-200, 150, 100, 50 };
    MusicPlayButton = (Rectangle) {(screenWidth/2)-50, 150, 100, 50 };
    MusicNextButton = (Rectangle) {(screenWidth/2)+100, 150, 100, 50 };
    MusicShuffleButton = (Rectangle) {(screenWidth/2)+250, 150, 100, 50 };
}
void DrawMusicMenu() {
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
}

void RandomInt() {
    int old = currentMusicIndex;  // Sauvegarde de la valeur actuelle
    do {
        currentMusicIndex = rand() % NUM_MUSIC_FILES;  // Génération d'un nouvel index
    } while (currentMusicIndex == old);  // Comparer les valeurs pour éviter la répétition
}