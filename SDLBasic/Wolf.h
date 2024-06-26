#pragma once
#include "Enemy.h"
class Wolf :
    public Enemy
{
public:
    Wolf(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar);
    void move(double vel);
    void update1(double frame);
    void update2(double frame);
    void update3(double frame);
    void prepare();
    void render();
    double distToSheep();
    void chase(double frame);
    static uint8_t phase;
    static uint8_t dead;
private:
    double speed;
    double cAngle;
    double chasePorpotion = 25.0, stopChasePorportion = 5.0;
    double chaseDistance, stopChaseDistance;
    bool chasing = false;
    const int chargeTime = 250; //take it takes to prepare charge, in ms
    const int prepAngle = 10; //radius of degrees to tilt by during charge prep
    double aSpeed = 0.05;
    int realHits = 0;
    double velX = 0.0;
    double velY = 0.0;
    double accel = 0.0001;
    double disX, disY, disSquared, tAccel, aMult;
};

