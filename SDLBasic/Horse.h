#pragma once
#include "Enemy.h"
class Horse :
    public Enemy
{
public:
    Horse(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    void update(double frame);
    void prepare();
private:
    double velX = 0.0;
    double velY = 0.0;
    double accel = 0.0001;
    double disX, disY, disSquared, tAccel;
};

