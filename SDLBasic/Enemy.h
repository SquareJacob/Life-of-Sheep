#pragma once
#include "GameObject.h"
#include "Sword.h"
#include "TextArea.h"

class Enemy :
    public GameObject
{
public:
    Enemy(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    ~Enemy();
    void damage(int dmg);
    //what to do when taking taking damage, returns if health <= 0
    bool damaged();
    //animation for defated, returns true when animation is done
    bool defeated(double frame);
    void prepare();
    void speak(std::string text);
    void render();
    void lookAt(GameObject* object);
    double ticker = 0;
protected:
    double knock = 0.0;
    Sword* sword;
    Sheep* sheep;
    int health, maxHealth;
    bool beenDamaged = false;
    Bar* bar;
    TextArea* textArea;
    std::string text;
    int hits = 0;
};

