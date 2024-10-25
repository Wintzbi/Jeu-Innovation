#include "raylib.h"

int main(void)
{
    // Initialisation de la fenêtre et des paramètres
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Exemple de caméra 2D avec WASD");

    // Définition de la caméra 2D
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f }; // Position de la cible (initialement à l'origine)
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Centre la caméra au milieu de l'écran
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Variable de vitesse pour déplacer la caméra
    const float cameraSpeed = 200.0f;

    SetTargetFPS(60); // Limite à 60 FPS

    // Boucle principale
    while (!WindowShouldClose())
    {
        // Gestion des entrées pour déplacer la caméra avec WASD
        if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * GetFrameTime();

        // Gestion du zoom de la caméra avec la molette de la souris
        if (IsKeyDown(KEY_Q)) camera.zoom += 0.05f;
        if (IsKeyDown(KEY_E)) camera.zoom -= 0.05f;

        // Limiter le zoom pour éviter des comportements imprévus
        if (camera.zoom < 0.1f) camera.zoom = 0.1f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;

        // Démarrage du rendu 2D avec la cam

        BeginMode2D(camera);
        
        
        // Dessin d'un rectangle pour représenter la cible suivie par la caméra

        EndMode2D();

        // Informations sur le contrôle de la cam
    }

    // Libération des ressources
    CloseWindow();

    return 0;
}
