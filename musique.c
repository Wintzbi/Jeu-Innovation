#include "musique.h"
// Déclaration des variables
Music music;  // Déclare une variable pour la musique

void InitMusic() {
    // Initialiser le système audio
    InitAudioDevice();

    // Charger la musique
    music = LoadMusicStream("Jeu-Innovation/musiques/01.mp3"); // Remplacez par le bon chemin
    PlayMusicStream(music);
}

void UpdateMusic() {
    // Mettre à jour le flux audio
    UpdateMusicStream(music);
}

void UnloadMusic() {
    // Décharger la musique
    UnloadMusicStream(music);
    CloseAudioDevice(); // Fermer le système audio si c'est la dernière utilisation
}