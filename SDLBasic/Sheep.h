#pragma once
#include "GameObject.h"
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
    Sheep(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, double aSpeed, int health);
    //Moves sheep in direction it is facing, or opposite if vel < 0. vel porpotional to change in position
    void move(double vel);
    //Rotates sheep amount porportional to vel. ccw is positive. Sets radians
    void rotate(double vel);
private:
    double speed; //movespeed porpotion
    double aSpeed; //rotation speed porpotion
    int health;
};

