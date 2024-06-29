#pragma once
#include "Enemy.h"
class Squirrel :
    public Enemy
{
public:
    Squirrel(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    void update(double frame);
    void render();
    void prepare();
    void acornsClear();
    static uint8_t phase;
    bool running = false;
    double acornTime = 0.0;
    class Acorn :
        public Enemy
    {
    public:
        Acorn(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, Squirrel* squirrel, double speed);
        bool update(double frame);
    private:
        double speed;
        Squirrel* squirrel;
    };
    std::vector<Acorn*> acorns;
private:
    double speed;
};

