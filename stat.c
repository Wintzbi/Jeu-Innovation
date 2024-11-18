#include "stat.h"
time_t start_time=0;
time_t current_time=0;
double elapsed=0;
int craftedObjects=0;
int pickedObject=0;

void InitStat(){
    time_t start_time = time(NULL); 
    // Temps de départ
    }
void UpdateStat(){
    time_t current_time = time(NULL);  // Temps actuel
    double elapsed = current_time - start_time ;  // Temps écoulé en secondes
}