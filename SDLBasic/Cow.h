#pragma once
#include "Enemy.h"
class Cow :
    public Enemy
{
public:
    Cow(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    //fades in for frame > 0, fade out for frame < 0, returns if opacity hits 0 or 1
    bool wiggle(double frame);
    double opacity = 0; //0 = invisible, 1 = visible
private:
};

