#include "Bull.h"

Bull::Bull(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
}

Bull::~Bull() {}

bool Bull::move(double vel) {
	return shift(vel * speed);
}

void Bull::update(double frame) {
	if (ticker <= 0) { //look at sheep
		angle = atan2(y - sheep->y, sheep->x - x) * 180.0 / PI;
		if (angle < 0) {
			angle += 360.0;
		}
		if (90 < angle && angle < 270) {
			flip = 2;
		}
		else {
			flip = 0;
		}
		cAngle = angle;
		ticker += frame / 3000;
		damage(5);
	}
	else if (ticker < TAU) { //wiggle
		angle = cAngle + prepAngle * sin(5.0 * ticker);
		ticker += TAU * frame / chargeTime / 5.0;
		ticker = ticker > TAU ? 7 : ticker;
		damage(10);
	}
	else if (ticker == 7){ //ready...
		ticker = 8;
		angle = cAngle;
		setRadians();
		move(frame);
		damage(10);
	}
	else if (ticker == 8) { //CHARGE!!
		if (move(frame)) {
			ticker = 9;
		}
		damage(30);
	}
	else if (ticker < 10) { //back up from wall
		move(-0.1 * frame);
		ticker += frame / 600;
		damage(15);
	}
	else { //restart
		ticker = -(float) health / (float) maxHealth;
		damage(15);
	}
}

void Bull::prepare() {
	Enemy::prepare();
	x = upperX;
	y = upperY;
	ticker = 0;
}