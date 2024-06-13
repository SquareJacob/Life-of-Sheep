#include "Horse.h"

Horse::Horse(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	knock = 3.0;
}

void Horse::update(double frame) {
	angle += frame * hypot(velX, velY);
	if (angle > 360.0) {
		angle -= 360.0;
	}
	ticker += frame;
	if (hits % 10 == 0 && hits != 0 && ticker < 7000.0) {
		ticker = 7000.0;
	}
	if (ticker < 7000.0) {
		disX = sheep->x - x;
		disY = sheep->y - y;
		disSquared = disX * disX + disY * disY;
		tAccel = frame * accel * (2.0 - (float)health / (float)maxHealth) / pow(disSquared, 0.5);
		velX += tAccel * disX;
		velY += tAccel * disY;
	}
	else if (ticker > 10000.0) {
		ticker = 0.0;
	}
	x += frame * velX;
	y += frame * velY;
	if (bound()) {
		velX = 0.0;
		velY = 0.0;
	}
	knockback(frame);
	damage(hypot(velX, velY) * 100.0);
}

void Horse::prepare() {
	Enemy::prepare();
	x = upperX;
	y = upperY;
	ticker = 0.0;
	velX = 0.0;
	velY = 0.0;
}