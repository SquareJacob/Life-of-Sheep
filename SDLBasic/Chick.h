#pragma once
#include "Enemy.h"
class Chick :
    public Enemy
{
public:
    Chick(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    void hatch();
    bool move(double vel);
    void update(double frame);
    bool alive = false;
    void render();
    void prepare();
    double stage = 0;
private:
    uint8_t behavior = 0; //chase, prepare, charge
    bool hatched = false;
    double speed;
};

