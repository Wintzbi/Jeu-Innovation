#ifndef MUSIQUE_H
#define MUSIQUE_H

#include "raylib.h"
#include <unistd.h>  // pour utiliser chdir()


// Déclaration des variables externes
extern int currentMusicIndex;
extern Music currentMusic;
extern bool currentChange;

// Prototypes des fonctions
void InitMusic();
void UpdateMusic();
void UnloadMusic();

// Définir le nombre de musiques dans la liste
#define NUM_MUSIC_FILES 6

// Liste des chemins des musiques (tableau de chaînes de caractères)
extern const char* musicFiles[NUM_MUSIC_FILES];

#endif
