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
    //what to do when taking taking damage, returns if health <= 0
    bool damaged(bool swordOut);
    //animation for defated, returns true when animation is done
    bool defeated(double frame);
protected:
    Sword* sword;
    Sheep* sheep;
    int health;
    bool beenDamaged = false;
    double ticker = 0;
    double knockback = 30;
};

