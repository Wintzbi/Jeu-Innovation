#include "raylib.h"
#include "cell.h"
#include "inventory.h"
#include "camera.h"

Camera2D camera = { 0 };
Rectangle player = { 0, 0, 0, 0 };

void setPlayerCamera() {
    if (IsKeyDown(KEY_RIGHT)) player.x += 2;
        else if (IsKeyDown(KEY_LEFT)) player.x -= 2;
            else if (IsKeyDown(KEY_UP)) player.y += 2;
                else if (IsKeyDown(KEY_UP)) player.y -= 2;

    camera.target = (Vector2){ player.x + 20.0f, player.y + 20.0f };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
}