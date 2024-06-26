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
    bool damage(int dmg, Enemy* object = NULL);
    //what to do when taking taking damage, returns if health <= 0
    bool damaged(int frame);
    //animation for defated, returns true when animation is done
    bool defeated(double frame);
    void prepare();
    void speak(std::string text);
    void render();
    void posBar();
    double ticker = 0;
    void clear();
    Bar* bar;
    int health, maxHealth;
    static std::vector<Enemy*> enemies;
protected:
    double knock = 0.0;
    bool poked = false;
    Sword* sword;
    Sheep* sheep;
    int16_t immunity = 0, immunityFrames = 300;
    TextArea* textArea;
    std::string text = "0";
    int hits = 0;
};

