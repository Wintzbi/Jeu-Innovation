#ifndef GLOBAL_H
#define GLOBAL_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "inventory.h"
#include "texture.h"
#include "input.h"
#include "musique.h"
#include "menu.h"
#include "craft.h"
#include "camera.h"
#include "stat.h"
#include <time.h>

extern GameScreen currentScreen;
extern bool isInventoryScreenOpen;
extern bool isOptionScreenOpen;
extern bool isCraftScreenOpen;
extern bool isMapScreenOpen;

int save();
int load();
#endif