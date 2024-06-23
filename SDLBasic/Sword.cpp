#include "Sword.h"

Sword::Sword(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, Sheep* sheep) :
	GameObject(spriteFile, renderer, height) {
	this->sheepWidth = sheep->width / 2;
	this->sheep = sheep;
	fSpeed = fBaseSpeed * height;
	reset();
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
		angle += swordAngle + 180;
		result = true;
	}
	else if (poke > 0) {
		angle += 270;
		if (poking()) {
			result = true;
		}
		poke -= frame;
		if (poke < 0) {
			poke = 0;
		}
	}
	else if (flying) {
		x -= fSpeed * sin(setRadians()) * (1 - flip);
		y -= fSpeed * cos(radians) * (1 - flip);
		swordOut = true;
		return;
	}
	x -= (float) (sheepWidth + width / 2) * sin(setRadians()) * (1 - flip);
	y -= (float) (sheepWidth + width / 2) * cos(radians) * (1 - flip);
	swordOut = result;
}

bool Sword::out() {
	return swordAngle > 0 || poke > 0 || flying;
}

void Sword::beginPoke() {
	poke = pokeTime;
}

void Sword::beginSwing() {
	swordAngle = 180;
}

void Sword::renderSwing(){
	if (poking() || swinging() || flying) {
		render();
	}
}

void Sword::updatePokeTime() {
	pokeTime /= upgrade;
}

void Sword::updateSwingDmg() {
	sDmg *= upgrade;
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
	else if (flying) {
		return fDmg;
	}
	else {
		return 0;
	}
}

void Sword::reset() {
	pokeTime = basePokeTime;
	sDmg = sBaseDmg;
}

void Sword::prepare() {
	poke = 0;
	swordAngle = 0;
	flying = false;
}
