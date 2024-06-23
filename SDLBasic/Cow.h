#pragma once
#include "Enemy.h"
class Cow :
    public Enemy
{
public:
    Cow(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, double edge);
    //fades in for frame > 0, fade out for frame < 0, returns if opacity hits 0 or 1
    bool wiggle(double frame);
    //0 = invisible, 1 = visible
    void setOpacity(double o);
    void updateOpacity(double o);
    void prepare();
    void update(double frame);
    void render();
    void boltsClear();
    uint8_t phase = 0; //0 = first scene, 1 = first battle, 2 = second scene, 3 = second battle, 4 = final scene
    int maxHealth2;
    class Bolt :
        public Enemy 
    {
    public:
        Bolt(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, Cow* cow);
        bool update(double frame);
        static double speed;
    private:
        Cow* cow;
    };
    std::vector<Bolt*> bolts;
private:
    double opacity = 0; //0 = invisible, 1 = visible
    double speed;
    double edge;
    uint8_t behavior = 0; //0 = teleport
};

