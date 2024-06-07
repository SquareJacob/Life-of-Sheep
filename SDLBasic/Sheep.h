#pragma once
#include "GameObject.h"
#include "Bar.h"
class Sheep :
    public GameObject
{
public:
    //Places sheep in topleft corner
    //spriteFile: path to get sprite from
    //renderer: what GameObject will be rendererd to
    //height: height of object; width will be based on porportions of original sprite
    //speed: movespeed porpotion
    //aSpeed: rotation speed porpotion
    Sheep(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, double aSpeed, int health, Bar* bar);
    //Moves sheep in direction it is facing, or opposite if vel < 0. vel porpotional to change in position
    void move(double vel);
    //Rotates sheep amount porportional to vel. ccw is positive. Sets radians
    void rotate(double vel);
    //make the sheep feel pain; returns whether the sheep felt pain (aka not in immunity frame)
    bool takeDmg(int dmg);
    void update(int frame);
    void prepare();
    int maxHealth;
    int health;
private:
    double speed; //movespeed porpotion
    double aSpeed; //rotation speed porpotion
    int immunityFrames = 500; //ms of immunity
    int immunity = 0;
    Bar* bar; //healthBar
};

