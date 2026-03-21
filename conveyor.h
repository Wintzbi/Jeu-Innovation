#ifndef CONVEYOR_H
#define CONVEYOR_H

#include "input.h"

void Convey(Conveyor *conv);
void Update_Conv(void);
void RemoveConveyor(int posX, int posY);
void RemoveBattery(int posX, int posY);

#endif
