#include "Sword.h"

Sword::Sword(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, double speed, int pokeTime, Sheep* sheep) :
	GameObject(spriteFile, renderer, height) {
	this->speed = speed;
	this->pokeTime = pokeTime;
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
	if (swordAngle > 0) {
		swordAngle -= (float) frame * speed;
		if (swordAngle < 0) {
			swordAngle = 0;
		}
		angle += swordAngle + 180;
	}
	if (poke > 0) {
		angle += 270;
		poke -= frame;
		if (poke < 0) {
			poke = 0;
		}
	}
	x -= (float) sheepWidth * sin(setRadians()) * (1 - flip);
	y -= (float) sheepWidth * cos(radians) * (1 - flip);
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
	if (swordAngle > 0 || poke > pokeTime * 4 / 5) {
		render();
	}
}
