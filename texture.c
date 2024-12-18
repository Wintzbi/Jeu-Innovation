#include "raylib.h"
#include "texture.h"

Texture2D copperTexture;
Texture2D copperVeinTexture;
Texture2D ironTexture;
Texture2D ironVeinTexture;
Texture2D coalTexture;
Texture2D coalVeinTexture;
Texture2D rockVeinTexture;
Texture2D rockTexture;
Texture2D steelTexture;

Texture2D sandVeinTexture;
Texture2D waterVeinTexture;
Texture2D oilVeinTexture;

Texture2D copperLingotTexture;
Texture2D ironLingotTexture;
Texture2D copperPlateTexture;
Texture2D copperRodTexture;
Texture2D ironPlateTexture;
Texture2D ironRodTexture;
Texture2D stretchTexture;
Texture2D gearTexture;
Texture2D copperCableTexture;
Texture2D sandDust;

Texture2D defaultTexture;
Texture2D drillTexture;
Texture2D purpleTexture;
Texture2D conveyorTexture;
Texture2D chestTexture;
Texture2D pipeTexture;
Texture2D furnaceTexture;
Texture2D solarpanelTexture;
Texture2D steamcentralTexture;
Texture2D piloneTexture;
Texture2D piloneEffectTexture;
Texture2D batteryTexture;
Texture2D pressTexture;
Texture2D lanternTexture;


void InitTexture() {
    //ATTENTION ne pas changer l'ordre car les id deffinissent les identifients des itemps
    copperTexture = LoadTexture("assets/copper_item.png");
    ironTexture = LoadTexture("assets/iron_item.png");
    coalTexture = LoadTexture("assets/coal_item.png");
    coalVeinTexture =LoadTexture("assets/coalVeinTexture.png");
    rockVeinTexture=LoadTexture("assets/rockVeinTexture.png");
    rockTexture=LoadTexture("assets/rock_item.png");
    steelTexture = LoadTexture("assets/steel_lingot.png");
    gearTexture = LoadTexture("assets/gear.png");
    sandVeinTexture =LoadTexture("assets/sand_texture.png");
    waterVeinTexture=LoadTexture("assets/water.png");
    oilVeinTexture=LoadTexture("assets/oilTexture.png");
    copperLingotTexture = LoadTexture("assets/copper_lingot-export.png");
    ironLingotTexture = LoadTexture("assets/iron_lingot-export.png");
    copperPlateTexture =LoadTexture("assets/copperplate_texture.png");
    copperRodTexture =LoadTexture("assets/copperstem_texture.png");
    ironPlateTexture =LoadTexture("assets/ironplate_texture.png");
    ironRodTexture =LoadTexture("assets/ironstem_texture.png");
    copperVeinTexture= LoadTexture("assets/copperVeinTexture.png");
    ironVeinTexture= LoadTexture("assets/ironVeinTexture.png");
    copperCableTexture = LoadTexture("assets/coppercable.png");
    sandDust = LoadTexture("assets/sand_dust.png");

    defaultTexture = LoadTexture("assets/grass_texture.png");
    drillTexture = LoadTexture("assets/drill.png");
    conveyorTexture= LoadTexture("assets/conveyor.png");
    pipeTexture =LoadTexture("assets/pipe.png");
    chestTexture = LoadTexture("assets/crate.png");
    furnaceTexture=LoadTexture("assets/furnace.png");
    purpleTexture = LoadTexture("assets/purple_texture.png");
    solarpanelTexture = LoadTexture("assets/solar_panel.png");
    steamcentralTexture = LoadTexture("assets/power_station.png");
    piloneTexture = LoadTexture("assets/coil.png");
    piloneEffectTexture = LoadTexture("assets/coil_effect.png");
    batteryTexture = LoadTexture("assets/battery.png");
    pressTexture = LoadTexture("assets/presse_hydrau_texture.png");
    stretchTexture = LoadTexture("assets/etireuse_texture.png");
    lanternTexture = LoadTexture("assets/lantern.png");
}

void UnloadAllTexture() {
    UnloadTexture(copperTexture);
    UnloadTexture(ironTexture);
    UnloadTexture(coalTexture);
    UnloadTexture(coalVeinTexture);
    UnloadTexture(rockVeinTexture);
    UnloadTexture(rockTexture);
    UnloadTexture(steelTexture);

    UnloadTexture(sandVeinTexture);
    UnloadTexture(waterVeinTexture);
    UnloadTexture(oilVeinTexture);

    UnloadTexture(copperLingotTexture);
    UnloadTexture(ironLingotTexture);
    UnloadTexture(copperPlateTexture);
    UnloadTexture(copperRodTexture);
    UnloadTexture(ironPlateTexture);
    UnloadTexture(ironRodTexture);

    UnloadTexture(defaultTexture);
    UnloadTexture(copperVeinTexture);
    UnloadTexture(ironVeinTexture);
    UnloadTexture(drillTexture);
    UnloadTexture(purpleTexture);
    UnloadTexture(conveyorTexture);
    UnloadTexture(pipeTexture);
    UnloadTexture(chestTexture);
    UnloadTexture(furnaceTexture);
    UnloadTexture(solarpanelTexture);
    UnloadTexture(steamcentralTexture);
    UnloadTexture(piloneTexture);
    UnloadTexture(batteryTexture);
    UnloadTexture(piloneEffectTexture);
    UnloadTexture(pressTexture);
    UnloadTexture(stretchTexture);
    UnloadTexture(gearTexture);
    UnloadTexture(sandDust);
    UnloadTexture(copperCableTexture);
    UnloadTexture(lanternTexture);


}