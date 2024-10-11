#include "menu.h"
#include "cell.h"

Rectangle playButton;

void ButtonPlay() {
    playButton = (Rectangle) {screenWidth / 2 - 50, screenHeight / 2 - 25, 100, 50 };
}