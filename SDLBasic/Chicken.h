#pragma once
#include "Enemy.h"
#include "Chick.h"
class Chicken :
    public Enemy
{
public:
    Chicken(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, double edge, SDL_Color fg, SDL_Color bg);
    bool move(double vel);
    void update(double frame);
    void prepare();
    void render();
private:
    double eggReady = 0;
    double aSpeed = 0.0;
    double aAccel = 0.0001;
    std::vector<Chick*> chicks;
    double speed;
    int chickHealth;
    int chickCost;
    uint8_t alive;
};

