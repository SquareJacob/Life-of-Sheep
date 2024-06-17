#include "Sheep.h"
#include <string>

Sheep::Sheep(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, double aSpeed, int health, Bar* bar) :
	GameObject(spriteFile, renderer, height) {
	this->speed = speed;
	this->aSpeed = aSpeed;
	this->health = health;
	this->bar = bar;
	this->maxHealth = health;
	cWidth *= 0.8;
	cHeight *= 0.8;
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
		health = health < 0 ? 0 : health;
		bar->setValue(health);
		return true;
	}
	return false;
}

void Sheep::update(int frame) {
	immunity -= frame;
	immunity = immunity < 0 ? 0 : immunity;
	knockback(frame);
}

void Sheep::prepare() {
	width /= 4;
	height /= 4;
	x = lowerX;
	y = lowerY;
	health = maxHealth;
	bar->setValue(health);
	setKnockback(0.0, 0.0, 0.0);
}

void Sheep::posBar() {
	bar->updatePos(x - width / 2 - GameObject::globalX, y + height / 2 - GameObject::globalY);
}
