#pragma once
#include "GameObject.h"
#include "Sword.h"
class Enemy :
    public GameObject
{
public:
    Enemy(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep);
    ~Enemy();
    void damage(int dmg);
    void damaged(bool swordOut);
protected:
    Sword* sword;
    Sheep* sheep;
    int health;
    bool beenDamaged = false;
};

