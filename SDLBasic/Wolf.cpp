#include "Wolf.h"
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define new DEBUG_NEW
#endif  

Wolf::Wolf(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = height / 200.0;
	knock = 3.0;
	chaseDistance = chasePorpotion * float(height);
	stopChaseDistance = stopChasePorportion * float(height);
}

void Wolf::move(double vel) {
	slide(vel * speed);
}

void Wolf::update1(double frame) {
	if (chasing) {
		chase(frame);
		if (distToSheep() < stopChaseDistance) {
			chasing = false;
			ticker = 0.0;
		}
		return;
	}
	if (ticker <= 0.0) { //look at sheep
		lookAt(sheep);
		cAngle = angle;
		ticker += frame / 1500;
		damage(5);
		if (distToSheep() > chaseDistance) {
			chasing = true;
		}
	}
	else if (ticker < TAU) { //wiggle
		angle = cAngle + prepAngle * sin(5.0 * ticker / pow(3.0, dead));
		ticker += TAU * frame / chargeTime / 5.0 * pow(3.0, dead);
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
	if (chasing) {
		chase(frame);
		if (distToSheep() < stopChaseDistance) {
			chasing = false;
			realHits = hits;
			ticker = 0.0;
		}
		return;
	}
	if (hits != realHits) {
		realHits = hits;
		flip = 2 - flip;
		angle += 180;
		if (angle > 360) {
			angle -= 360;
		}
	}

	turnTowards(sheep, aSpeed * (2.0 - 1.0 * (float)health / (float)maxHealth) * (float)(dead + 1) * frame);
	ticker += frame / 1000.0;
	if (ticker < 7.0) {
		move(frame * (float)(dead + 1));
	}
	else if (ticker > 9.97) {
		ticker = 0.0;
	}
	damage(15);
	if (distToSheep() > chaseDistance) {
		chasing = true;
	}
}

void Wolf::update3(double frame) {
	if (chasing) {
		chase(frame);
		if (distToSheep() < stopChaseDistance) {
			chasing = false;
			velX = 0.0;
			velY = 0.0;
		}
		return;
	}
	angle += frame * hypot(velX, velY);
	if (angle > 360.0) {
		angle -= 360.0;
	}
	ticker += frame / 1000.0;
	if (ticker < 5.0) {
		disX = sheep->x - x;
		disY = sheep->y - y;
		disSquared = disX * disX + disY * disY;
		aMult = (2.0 - (float)health / (float)maxHealth) * (float)(dead + 1);
		tAccel = frame * accel * aMult / pow(disSquared, 0.5);
		velX += tAccel * disX;
		velY += tAccel * disY;
	}
	else if (ticker > 6.91) {
		ticker = 0.0;
	}
	knockback(frame);
	damage(hypot(velX, velY) * 50.0 / aMult + 5);
	x += frame * velX;
	y += frame * velY;
	if (distToSheep() > chaseDistance / (float)(dead + 1)) {
		chasing = true;
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
	dead = 0;
}

double Wolf::distToSheep() {
	return hypot(x - sheep->x, y - sheep->y);
}

void Wolf::chase(double frame) {
	lookAt(sheep);
	move(2.0 * frame);
}

uint8_t Wolf::phase = 0;
uint8_t Wolf::dead = 0;