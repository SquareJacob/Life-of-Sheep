#include "Sheep.h"
#include <string>

Sheep::Sheep(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, double aSpeed, int health, TextArea* healthText) :
	GameObject(spriteFile, renderer, height) {
	this->speed = speed;
	this->aSpeed = aSpeed;
	this->health = health;
	this->healthText = healthText;
}

void Sheep::move(double vel) {
	shift(vel * speed);
}

void Sheep::rotate(double vel) {
	tilt(aSpeed * vel);
}

bool Sheep::takeDmg(int dmg) {
	if (immunity == 0) {
		immunity = immunityFrames;
		health -= dmg;
		healthText->changeText(std::to_string(health));
		return true;
	}
	return false;
}

void Sheep::update(int frame) {
	immunity -= frame;
	immunity = immunity < 0 ? 0 : immunity;
}
