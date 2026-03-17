#include "ui.h"
#include "cell.h"
#include "camera.h"
#include "texture.h"
#include "raylib.h"

// ─── Minimap ─────────────────────────────────────────────────────────────────

// Calcule le rect du viewport de la caméra en coordonnées minimap
static Rectangle GetViewportRect(void) {
    float scale   = 0.50f;
    float mapX    = screenWidth  / 4.0f;
    float mapY    = 100.0f;
    float tileW   = cellSize * scale;
    float tileH   = cellSize * scale;

    // Centre de la caméra en coordonnées minimap
    float cx = mapX + (camera.target.x / cellSize) * tileW;
    float cy = mapY + (camera.target.y / cellSize) * tileH;

    // Taille du viewport visible en coordonnées monde
    float visW = (float)screenWidth  / camera.zoom;
    float visH = (float)screenHeight / camera.zoom;

    float rw = (visW / cellSize) * tileW;
    float rh = (visH / cellSize) * tileH;

    return (Rectangle){ cx - rw / 2.0f, cy - rh / 2.0f, rw, rh };
}

void DrawMap(void) {
    float scale  = 0.50f;
    float tileW  = cellSize * scale;
    float tileH  = cellSize * scale;
    float mapX   = screenWidth / 4.0f;
    float mapY   = 100.0f;

    Texture2D RefTexture = chestTexture;
    float scaleX = tileW / RefTexture.width;
    float scaleY = tileH / RefTexture.height;
    float ts     = (scaleX < scaleY) ? scaleX : scaleY;
    Rectangle sourceRec = { 0, 0, (float)RefTexture.width, (float)RefTexture.height };

    // Dessiner toutes les cellules à l'échelle minimap
    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
            Cell cell = grid[r][c];
            float rot = (float)GetDirectionAngle(cell.dir);

            Rectangle destRec = {
                mapX + cell.i * tileW + tileW / 2.0f,
                mapY + cell.j * tileH + tileH / 2.0f,
                RefTexture.width  * ts,
                RefTexture.height * ts
            };
            Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

            DrawTexturePro(cell.texture,      sourceRec, destRec, origin, 0.0f, WHITE);
            if (cell.up_texture.id   != 0)
                DrawTexturePro(cell.up_texture,   sourceRec, destRec, origin, rot,  WHITE);
            if (cell.move_texture.id != 0)
                DrawTexturePro(cell.move_texture, sourceRec, destRec, origin, 0.0f, WHITE);
        }
    }

    // Titre
    DrawRectangle((int)(screenWidth / 2.0f) - 400, 10, 800, 60, Fade(DARKGRAY, 0.7f));
    DrawText("MAP", (int)(screenWidth / 2.0f) - 390, 11, 60, Fade(WHITE, 0.7f));

    // Viewport de la caméra (rectangle rouge)
    Rectangle vp = GetViewportRect();
    DrawRectangleLines((int)vp.x, (int)vp.y, (int)vp.width, (int)vp.height, RED);

    // Filtre nuit
    int bright = DayAndNight();
    if (bright > 0) {
        Rectangle screenRec = { mapX, mapY, tileW * COL, tileH * ROW };
        DrawRectangleRec(screenRec, (Color){ 0, 0, 0, (unsigned char)bright });
    }

    // ─── Crédits (gauche) ────────────────────────────────────────────────────
    DrawText("CREDITS :",           10, 100, 35, Fade(DARKGRAY, 0.7f));
    DrawText("Jeu créé durant le ", 10, 140, 25, Fade(DARKGRAY, 0.7f));
    DrawText("parcours Innovation,",10, 170, 25, Fade(DARKGRAY, 0.7f));
    DrawText("du premier semestre", 10, 200, 25, Fade(DARKGRAY, 0.7f));
    DrawText("2024 par :",           10, 230, 25, Fade(DARKGRAY, 0.7f));
    DrawText("Leynaïck Duthieuw,",  10, 260, 25, Fade(DARKGRAY, 0.7f));
    DrawText("Charlilou Maes",      10, 290, 25, Fade(DARKGRAY, 0.7f));
    DrawText("Mathys Karin-Grisoni",10, 320, 25, Fade(DARKGRAY, 0.7f));

    // ─── Contrôles (droite) ──────────────────────────────────────────────────
    int cx = (int)(tileW * COL + mapX) + 10;
    DrawText("CONTROLS :",          cx, 100, 35, Fade(DARKGRAY, 0.7f));
    DrawText("Déplacements : ZQSD", cx, 140, 25, Fade(DARKGRAY, 0.7f));
    DrawText("Option : ESC",        cx, 170, 25, Fade(DARKGRAY, 0.7f));
    DrawText("Inventaire : E",      cx, 200, 25, Fade(DARKGRAY, 0.7f));
    DrawText("Craft : C",           cx, 230, 25, Fade(DARKGRAY, 0.7f));
    DrawText("Map : M",             cx, 260, 25, Fade(DARKGRAY, 0.7f));
}

void DrawHUD(void) {
    // Réservé — future barre de statut en jeu
}
