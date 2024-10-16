#include "musique.h"

// Définir la variable index pour suivre la musique en cours
int currentMusicIndex = 0;
bool currentChange = false;
bool MusicPlay=true;


void DrawEscapePage(){
    DrawRectangle(100, 10, screenWidth-200, 60, DARKGRAY);
    DrawText(TextFormat("OPTIONS"), 330, 11, 60, WHITE); // Display quantity
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
    "musiques/06.mp3"
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
    if (MusicPlay) PlayMusicStream(currentMusic);
    else if (!MusicPlay) PlayMusicStream(currentMusic);

    // Mettre à jour le flux audio
    UpdateMusicStream(currentMusic);

    // Vérifier si la musique est terminée et qu'il n'y a pas de changement en attente
    if (!IsMusicStreamPlaying(currentMusic) && (!currentChange)) {
        // Décharger la musique actuelle
        UnloadMusicStream(currentMusic);

        // Passer à la musique suivante
        currentMusicIndex++;
        if (currentMusicIndex >= NUM_MUSIC_FILES) {
            currentMusicIndex = 0;  // Revenir à la première musique
        }

        // Charger et jouer la nouvelle musique
        currentMusic = LoadMusicStream(musicFiles[currentMusicIndex]);
        PlayMusicStream(currentMusic);
    }

    // Gérer le changement de musique manuel
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
    DrawRectangle(100, 80, screenWidth-200, 60, DARKGRAY);
    DrawText(TextFormat("Musique : %s",&musicFiles[currentMusicIndex][9]), 110, 81, 60, WHITE);
}
// Variable pour le bouton Play
Rectangle MusicPlayButton,MusicPreviewButton,MusicNextButton;

// Fonction pour initialiser le bouton Play
void MusicButton() {
    MusicPlayButton = (Rectangle) {100, 150, 100, 50 };
    MusicPreviewButton = (Rectangle) {210, 150, 100, 50 };
    MusicNextButton = (Rectangle) {320, 150, 100, 50 };
}
void DrawMusicMenu() {
    MusicButton();

    // Dessiner le bouton "Play"
    if (CheckCollisionPointRec(GetMousePosition(), MusicPlayButton)) {
        DrawRectangleRec(MusicPlayButton, LIGHTGRAY); } // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ){
            MusicPlay=!MusicPlay;
        }
    else {
        DrawRectangleRec(MusicPlayButton, GRAY);}

   if (CheckCollisionPointRec(GetMousePosition(),MusicPreviewButton )) {
        DrawRectangleRec(MusicPreviewButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            if (currentMusicIndex>0){
                currentMusicIndex--;
                currentChange=true;
            }
            else currentMusicIndex=NUM_MUSIC_FILES-1;
        }
        } 
    else {
        DrawRectangleRec(MusicPreviewButton, GRAY);  // Couleur normale

    }

    if (CheckCollisionPointRec(GetMousePosition(), MusicNextButton)) {
        DrawRectangleRec(MusicNextButton, LIGHTGRAY);  // Couleur survolée
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            if (currentMusicIndex<NUM_MUSIC_FILES-1){
                currentMusicIndex++;
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
    DrawText(TextFormat("PAUSE /\nPLAY"), 100, 150, 20, WHITE);
    DrawText(TextFormat("Preview"), 210, 150, 20, WHITE);
    DrawText(TextFormat("Next "), 320, 150, 20, WHITE);

}
