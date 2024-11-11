#ifndef STAT_H
#define STAT_H
#include <time.h>


extern int craftedObjects;
extern double elapsed;
extern time_t start_time;
void InitStat();
void UpdateStat();

#endif