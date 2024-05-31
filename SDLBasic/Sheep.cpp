#include "Sheep.h"


Sheep::Sheep(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, double aSpeed, int health) :
	GameObject(spriteFile, renderer, height) {
	this->speed = speed;
	this->aSpeed = aSpeed;
	this->health = health;
}

void Sheep::move(double vel) {
	shift(vel * speed);
}

void Sheep::rotate(double vel) {
	tilt(aSpeed * vel);
}
