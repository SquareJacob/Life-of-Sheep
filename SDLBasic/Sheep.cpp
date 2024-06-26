#include "Sheep.h"
#include <string>
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define new DEBUG_NEW
#endif  

Sheep::Sheep(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, double aSpeed, int health, Bar* bar) :
	GameObject(spriteFile, renderer, height) {
	this->speed = speed;
	this->aSpeed = aSpeed;
	this->health = health;
	this->bar = bar;
	this->maxHealth = health;
	flightRechargeTime = baseFlightRechargeTime;
	cWidth *= 0.8;
	cHeight *= 0.8;
	flightRecharge = flightRechargeTime;
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

	if (flyable) {
		if (flightRecharge > 0) {
			flightRecharge -= frame;
			if (flightRecharge == 0) {
				flightRecharge--;
			}
		}
		else if (flightRecharge < 0) {
			setSprite(1);
			flightRecharge = 0;
		}
	}

	knockback(frame);
}

void Sheep::prepare(bool farm) {
	if (farm) {
		x = lowerX;
		y = lowerY;
	}
	else {
		x = gameWidth / 2;
		y = gameHeight / 2;
	}
	health = maxHealth;
	bar->setValue(health);
	setKnockback(0.0, 0.0, 0.0);
}

void Sheep::posBar() {
	bar->updatePos(x - width / 2 - GameObject::globalX, y + height / 2 - GameObject::globalY);
}

void Sheep::resetFlight() {
	flightRecharge = flightRechargeTime;
	setSprite(0);
}

void Sheep::resetFling() {
	flightRechargeTime = baseFlightRechargeTime;
}

void Sheep::updateFlingTime() {
	flightRechargeTime /= upgrade;
}
 