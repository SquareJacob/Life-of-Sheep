#pragma once
#include "GameObject.h"
#include "Sheep.h"
class Sword :
    public GameObject

{
public:
    //Places sword in topleft corner intially.
    //spriteFile: path to get sprite from
    //renderer: what GameObject will be rendererd to
    //height: height of object; width will be based on porportions of original sprite
    //speed: porpotional to speed of swing
    //pokeTime: time between pokes in ms
    //sheep: sheep :)
    Sword(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, Sheep* sheep);
    //Set position and angle relative to sheep
    void sheepify();
    //progress current swing or poke, and move sword based on swordAngle
    void swing(int frame);
    //whether or not sword is currently midaction or on cooldown
    bool out();
    void beginPoke();
    void beginSwing();
    //renders sword swing if mid-action
    void renderSwing();
    //updates linearly
    void updatePokeTime();
    //updates linearly
    void updateSwingDmg();
    bool poking();
    bool swinging();
    int damage();
    void reset();
    void prepare();
    bool swordOut = false;
private:
    double speed = 0.15; //porpotional to speed of swing
    double sAngle = 0.0; //sheep Angle
    double swordAngle = 0.0; //sword Angle relative to sheep
    uint16_t pokeTime, pDmg = 20, sDmg, sBaseDmg = 80, basePokeTime = 220;
    int16_t poke = 0;
    double upgrade = 1.1;
    int sheepWidth; //actually sheepWidth/2; useful constant
    Sheep* sheep; //sheep :)
};

