#include "ui.h"
#include "cell.h"
#include "camera.h"
#include "texture.h"
#include "stat.h"
#include "input.h"
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

void DrawMap(int brightValue) {
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
    if (brightValue > 0) {
        Rectangle screenRec = { mapX, mapY, tileW * COL, tileH * ROW };
        DrawRectangleRec(screenRec, (Color){ 0, 0, 0, (unsigned char)brightValue });
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

void DrawStat(void) {
    int cx = GetScreenWidth() / 2;
    DrawRectangle(cx - 400, 250, 800, 60, DARKGRAY);
    DrawText("Statistiques",     cx - 390, 250, 60, WHITE);
    DrawText(TextFormat("Craft : %d",          craftedObjects), cx - 390, 350, 50, LIGHTGRAY);
    DrawText(TextFormat("Objets ramassé : %d", pickedObject),  cx - 390, 420, 50, LIGHTGRAY);
    DrawText(TextFormat("Temps écoulé : %d",   days),          cx - 390, 490, 50, LIGHTGRAY);
}

// ─── Mode info (F2) ──────────────────────────────────────────────────────────
// Dessine un tooltip au survol d'une machine ou d'un convoyeur.
// À appeler dans le bloc BeginMode2D / EndMode2D.

static void DrawTooltip(int worldX, int worldY, const char **lines, int count) {
    int   fontSize = 6;
    float spacing  = 0.5f;
    Font  font     = GetFontDefault();
    int   padding  = fontSize / 4;
    int   lineH    = (int)(fontSize * 1.25f);

    int w = 0;
    for (int i = 0; i < count; i++) {
        Vector2 sz = MeasureTextEx(font, lines[i], fontSize, spacing);
        if ((int)sz.x > w) w = (int)sz.x;
    }
    w += padding * 2;
    int h = count * lineH + padding * 2;

    int tx = worldX * cellSize + cellSize + 2;
    int ty = worldY * cellSize - h / 2;

    DrawRectangle(tx, ty, w, h, Fade(BLACK, 0.75f));
    DrawRectangleLines(tx, ty, w, h, Fade(WHITE, 0.4f));
    for (int i = 0; i < count; i++)
        DrawTextEx(font, lines[i],
                   (Vector2){tx + padding, ty + padding + i * lineH},
                   fontSize, spacing, WHITE);
}

void DrawInfoTooltip(void) {
    if (!infoMode) return;

    Vector2 mousePos  = GetMousePosition();
    Vector2 worldPos  = GetScreenToWorld2D(mousePos, camera);
    int posX = (int)(worldPos.x / cellSize);
    int posY = (int)(worldPos.y / cellSize);

    if (!IndexIsValid(posX, posY)) return;

    const char *lines[12];
    char buf[12][48];
    int n = 0;

    // ── Panneau solaire ───────────────────────────────────────────────────
    if (grid[posX][posY].up_texture.id == solarpanelTexture.id) {
        int darkness = DayAndNight();
        int pct      = ((64 - darkness) * 100) / 64;
        snprintf(buf[n], 48, "Panneau Solaire");           lines[n] = buf[n]; n++;
        snprintf(buf[n], 48, "Production : %dA", (SOLAR_PER_PANEL * (64 - darkness)) / 64);
                                                           lines[n] = buf[n]; n++;
        snprintf(buf[n], 48, "Luminosite : %d%%", pct);   lines[n] = buf[n]; n++;
        DrawTooltip(posX, posY, lines, n);
        return;
    }

    // ── Foreuse ───────────────────────────────────────────────────────────
    for (int i = 0; i < numForeuses; i++) {
        if (ListeForeuse[i].i != posX || ListeForeuse[i].j != posY) continue;
        snprintf(buf[n], 48, "Foreuse"); lines[n] = buf[n]; n++;
        snprintf(buf[n], 48, "Stock : %d", ListeForeuse[i].q); lines[n] = buf[n]; n++;
        DrawTooltip(posX, posY, lines, n);
        return;
    }

    // ── Machines processeurs ──────────────────────────────────────────────
    Machine *lists[]   = { ListeFurnace, ListeHydraulic, ListeEttireuse };
    int      counts[]  = { numFurnaces,  numHydraulics,  numEttireuses  };
    const char *names[]= { "Four",       "Presse",       "Etireuse"     };
    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < counts[t]; i++) {
            if (lists[t][i].i != posX || lists[t][i].j != posY) continue;
            Machine *m = &lists[t][i];
            snprintf(buf[n], 48, "%s", names[t]);           lines[n] = buf[n]; n++;
            snprintf(buf[n], 48, "Energie : %d",  m->energy_q);   lines[n] = buf[n]; n++;
            snprintf(buf[n], 48, "Materiau: %d",  m->material_q); lines[n] = buf[n]; n++;
            snprintf(buf[n], 48, "Produit : %d",  m->final_q);    lines[n] = buf[n]; n++;
            DrawTooltip(posX, posY, lines, n);
            return;
        }
    }

    // ── Centrale vapeur ───────────────────────────────────────────────────
    for (int i = 0; i < numSteams; i++) {
        if (ListeSteam[i].i != posX || ListeSteam[i].j != posY) continue;
        snprintf(buf[n], 48, "Centrale Vapeur");          lines[n] = buf[n]; n++;
        snprintf(buf[n], 48, "Combustible : %d", ListeSteam[i].energy_q);  lines[n] = buf[n]; n++;
        snprintf(buf[n], 48, "Eau         : %d", ListeSteam[i].material_q);lines[n] = buf[n]; n++;
        snprintf(buf[n], 48, "Energie out : %d", ListeSteam[i].final_q);   lines[n] = buf[n]; n++;
        DrawTooltip(posX, posY, lines, n);
        return;
    }

    // ── Batterie ──────────────────────────────────────────────────────────
    for (int k = 0; k < MAX_BATTERY; k++) {
        if (!ListeBattery[k].placed) continue;
        if (ListeBattery[k].i != posX || ListeBattery[k].j != posY) continue;
        snprintf(buf[n], 48, "Batterie");                              lines[n] = buf[n]; n++;
        snprintf(buf[n], 48, "Charge : %d/100", ListeBattery[k].q);  lines[n] = buf[n]; n++;
        DrawTooltip(posX, posY, lines, n);
        return;
    }

    // ── Convoyeur ─────────────────────────────────────────────────────────
    for (int k = 0; k < MAX_CONVEYOR; k++) {
        if (!ListeConveyor[k].placed) continue;
        if (ListeConveyor[k].i != posX || ListeConveyor[k].j != posY) continue;
        if (ListeConveyor[k].texture.id == piloneTexture.id) {
            snprintf(buf[n], 48, "Pylone");                              lines[n] = buf[n]; n++;
            snprintf(buf[n], 48, "Charge : %d/%d A",
                ListeConveyor[k].load, ListeConveyor[k].max_load);      lines[n] = buf[n]; n++;
        } else {
            snprintf(buf[n], 48, "Convoyeur");                          lines[n] = buf[n]; n++;
            snprintf(buf[n], 48, "Items   : %d/%d",
                ListeConveyor[k].amount, ListeConveyor[k].capacity);    lines[n] = buf[n]; n++;
        }
        DrawTooltip(posX, posY, lines, n);
        return;
    }
}