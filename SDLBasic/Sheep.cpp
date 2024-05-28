#include "Sheep.h"


Sheep::Sheep(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, double aSpeed) :
	GameObject(spriteFile, renderer, height) {
	this->speed = speed;
	this->aSpeed = aSpeed;
}

void Sheep::move(double vel) {
	x += vel * speed * cos(radians) * (1 - 2 * flip);
	y -= vel * speed * sin(radians) * (1 - 2 * flip);
}

void Sheep::rotate(double vel) {
	angle += vel * aSpeed;
	angle = std::fmod(angle, 360.0);
	setRadians();
}
