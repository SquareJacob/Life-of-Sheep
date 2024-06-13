#include "Sword.h"

Sword::Sword(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, int pokeTime, Sheep* sheep, int pDmg, int sDmg) :
	GameObject(spriteFile, renderer, height) {
	this->speed = speed;
	this->pokeTime = pokeTime;
	this->pDmg = pDmg;
	this->sDmg = sDmg;
	this->sheepWidth = sheep->width / 2;
	this->sheep = sheep;
	poke = 0;
	swordAngle = 0.0;
	sAngle = 0.0;
}

void Sword::sheepify() {
	sAngle = sheep->angle;
	angle = sheep->angle;
	x = sheep->x;
	y = sheep->y;
	flip = 2 * sheep->flip;
}

void Sword::swing(int frame) {
	bool result = false;
	if (swordAngle > 0) {
		swordAngle -= (float) frame * speed;
		if (swordAngle < 0) {
			swordAngle = 0;
		}
		angle += (1 - flip) * swordAngle + 90 * (2 - flip);
		result = true;
	}
	if (poke > 0) {
		angle += 270;
		if (poking()) {
			result = true;
		}
		poke -= frame;
		if (poke < 0) {
			poke = 0;
		}
	}
	x -= (float) (sheepWidth + width / 2) * sin(setRadians()) * (1 - flip);
	y -= (float) (sheepWidth + width / 2) * cos(radians) * (1 - flip);
	swordOut = result;
}

bool Sword::out() {
	return swordAngle > 0 || poke > 0;
}

void Sword::beginPoke() {
	poke = pokeTime;
}

void Sword::beginSwing() {
	swordAngle = 180;
}

void Sword::renderSwing(){
	if (poking() || swinging()) {
		render();
	}
}

void Sword::updatePokeTime(int amount) {
	pokeTime += amount;
}

void Sword::updateSwingDmg(int amount) {
	sDmg += amount;
}

bool Sword::poking() {
	return poke > pokeTime * 4 / 5;
}

bool Sword::swinging() {
	return swordAngle > 0;
}

int Sword::damage() {
	if (poking()) {
		return pDmg;
	}
	else if (swinging()) {
		return sDmg;
	}
	else {
		return 0;
	}
}
