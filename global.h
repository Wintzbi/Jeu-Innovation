#ifndef GLOBAL_H
#define GLOBAL_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "inventory.h"
#include "global.h"
#include "texture.h"
#include "input.h"

Cell grid[ROW][COL];

#include "menu.h"

Cell grid[ROW][COL];
typedef enum GameScreen { MENU = 0, GAME } GameScreen;

#endif