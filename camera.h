#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

// Taille de la zone visible par la caméra
extern const int visibleCols;
extern const int visibleRows;

// Fonction pour mettre à jour la caméra
void UpdateTheCamera();

// Fonction pour dessiner la carte avec la caméra
void DrawMap();

// Fonction pour gérer la mise à jour et le dessin de la caméra
void CameraUpdateAndDraw();

#endif
