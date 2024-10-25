#ifndef MUSIQUE_H
#define MUSIQUE_H
#include "raylib.h"
#include "cell.h"
#include <unistd.h>  // pour utiliser chdir()


// Déclaration des variables externes
extern int currentMusicIndex;
extern Music currentMusic;
extern bool currentChange;
extern bool MusicPlay;
extern bool MusicLoop;
extern bool MusicShuffle;

// Prototypes des fonctions
void InitMusic();
void UpdateMusic();
void UnloadMusic();
void DrawEscapePage();

// Définir le nombre de musiques dans la liste
#define NUM_MUSIC_FILES 7
// Variable pour les bouttons

// Liste des chemins des musiques (tableau de chaînes de caractères)
extern const char* musicFiles[NUM_MUSIC_FILES];
void DrawMusicMenu();
void DrawMusic();
void MusicButton();
void RandomInt() ;

#endif
