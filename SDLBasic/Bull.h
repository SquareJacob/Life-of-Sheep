#pragma once
#include "Enemy.h"
#include "Sword.h"
#define TAU PI * 2
class Bull :
    public Enemy
{
public:
	//Places object in topleft corner intially.
	//spriteFile: path to get sprite from
	//renderer: what GameObject will be rendererd to
	//height: height of object; width will be based on porportions of original sprite
    Bull(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep);
	~Bull();
	//Moves bull in direction it is facing, or opposite if vel < 0. vel porpotional to change in position
	bool move(double vel);
	void update(double frame);
private:
	const int chargeTime = 250; //take it takes to prepare charge, in ms
	const int prepAngle = 10; //radius of degrees to tilt by during charge prep
	const double speed = 0.3; //charge speed porportion
	double cAngle; //angle set to charge in
	double cStep; //current step charge is in
};

