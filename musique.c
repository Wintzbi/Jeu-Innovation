#include "musique.h"
// Déclaration des variables
Music music;  // Déclare une variable pour la musique

void InitMusic() {
    // Initialiser le système audio
    InitAudioDevice();

    // Charger la musique
    music = LoadMusicStream("/home/leynaick/parcours/game/musiques/main theme - 03:10:2024 11.49.mp3"); // Remplacez par le bon chemin
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