#ifndef ENERGY_H
#define ENERGY_H

// ─── Système énergétique ──────────────────────────────────────────────────────
// BFS depuis une machine vers les sources (Steam, Batterie, Solaire).
// Les pylônes servent de relais avec une capacité max de max_load ampères.

#define SOLAR_PER_PANEL 3  // production par panneau par tick (plein jour)

int HasEnergySource(int x, int y, int range);
int RequestEnergy(int x, int y, int amount);
int IsEnergieNear(int x, int y, int range);
int FindNearestBattery(int x, int y);
int FindNearestSteam(int x, int y);
void UpdateBattery(void);
void AddSolarPanel(int i, int j);
void RemoveSolarPanel(int i, int j);
void ReloadSolarPool(void);
void RebuildSolarPanels(void);
void DebugEnergy(void);

#endif

// Appelé depuis Update_Conv — recharge solarPool selon l'heure
void ChargeSolarPool(int darkness);
int  GetSolarPool(void);
