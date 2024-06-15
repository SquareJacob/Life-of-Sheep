#pragma once
#include "Enemy.h"
class Dog :
    public Enemy
{
public:
    Dog(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    bool move(double vel);
    void prepare();
    void update(double frame);
    bool damaged(int frame);
private:
    uint8_t behavior = 0; //0 = chase, 1 = staggered
    double speed;
    double healthFraction = 0;
    double sAngle;
    double baseAngleSpeed = 0.1;
    double aSpeed;
    bool staggered = false; //whether it was staggered at this many hits
    int realHits = 0;
};

