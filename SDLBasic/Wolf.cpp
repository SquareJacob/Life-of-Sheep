#include "Wolf.h"

Wolf::Wolf(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = height / 200.0;
	knock = 3.0;
}

void Wolf::move(double vel) {
	slide(vel * speed);
}

void Wolf::update1(double frame) {
	if (ticker <= 0.0) { //look at sheep
		lookAt(sheep);
		cAngle = angle;
		ticker += frame / 1500;
		damage(5);
	}
	else if (ticker < TAU) { //wiggle
		angle = cAngle + prepAngle * sin(5.0 * ticker);
		ticker += TAU * frame / chargeTime / 5.0;
		ticker = ticker > TAU ? 7 : ticker;
		damage(10);
	}
	else if (ticker == 7.0) { //ready...
		ticker = 8.0;
		angle = cAngle;
		setRadians();
		move(frame);
		damage(10);
	}
	else if (ticker < 9.0) {
		move(2.0 * frame);
		damage(30);
		ticker += frame / 1000;

	}
	else {
		ticker = -(float)health / (float)maxHealth;
	}
}

void Wolf::update2(double frame) {
	if (hits != realHits) {
		realHits = hits;
		flip = 2 - flip;
		angle += 180;
		if (angle > 360) {
			angle -= 360;
		}
	}
	turnTowards(sheep, aSpeed * (3.0 - 2.0 * (float)health / (float)maxHealth) * frame);
	move(frame);
	damage(15);
}

void Wolf::update3(double frame) {
	angle += frame * hypot(velX, velY);
	if (angle > 360.0) {
		angle -= 360.0;
	}
	disX = sheep->x - x;
	disY = sheep->y - y;
	disSquared = disX * disX + disY * disY;
	tAccel = frame * accel * (2.0 - (float)health / (float)maxHealth) / pow(disSquared, 0.5);
	velX += tAccel * disX;
	velY += tAccel * disY;
	knockback(frame);
	damage(hypot(velX, velY) * 50.0 + 10);
	x += frame * velX;
	y += frame * velY;
	ticker += frame / 1000.0;
	if (ticker > 5.0) {
		ticker = 0.0;
		velX = 0.0;
		velY = 0.0;
	}
}

void Wolf::render() {
	Enemy::render();
	if (phase == 1) {
		posBar();
		bar->render();
	}
}

void Wolf::prepare() {
	Enemy::prepare();
	velX = 0.0;
	velY = 0.0;
	realHits = 0;
}

uint8_t Wolf::phase = 0;