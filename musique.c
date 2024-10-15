#include "musique.h"

// Définir la variable index pour suivre la musique en cours
int currentMusicIndex = 0;
bool currentChange = false;

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
    // Mettre à jour le flux audio
    UpdateMusicStream(currentMusic);

    // Vérifier si la musique est terminée et qu'il n'y a pas de changement en attente
    if (!IsMusicStreamPlaying(currentMusic) && !currentChange) {
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
