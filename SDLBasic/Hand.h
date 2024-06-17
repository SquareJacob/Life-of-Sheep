#pragma once
#include "Enemy.h"
class Hand :
    public Enemy
{
public:
    Hand(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    //returns if already on target
    bool moveTowards(GameObject* object, double frame);
private:
    double speed;
    double aSpeed = 0.1;
    static double oAngle;
};

