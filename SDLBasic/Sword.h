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
    Sword(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, int pokeTime, Sheep* sheep);
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
private:
    double speed; //porpotional to speed of swing
    double sAngle; //sheep Angle
    double swordAngle; //sword Angle relative to sheep
    int poke, pokeTime;
    int sheepWidth; //actually sheepWidth/2; useful constant
    Sheep* sheep; //sheep :)
};

