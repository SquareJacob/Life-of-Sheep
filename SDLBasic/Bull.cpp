#include "Bull.h"

Bull::Bull(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = (float) height / 333.3333333;
}

Bull::~Bull() {}

bool Bull::move(double vel) {
	return shift(vel * speed);
}

void Bull::update(double frame) {
	if (ticker <= 0.0) { //look at sheep
		lookAt(sheep);
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
	else if (ticker == 7.0){ //ready...
		ticker = 8.0;
		angle = cAngle;
		setRadians();
		move(frame);
		damage(10);
	}
	else if (ticker == 8.0) { //CHARGE!!
		if (move(frame)) {
			//std::cout << cos(radians) << ' ' << sin(radians) << std::endl;
			if (x == lowerX || x == upperX) {
				if (abs(cos(radians)) > 0.1) {
					ticker = 9.0;
				}
			}
			if (y == lowerY || y == upperY) {
				if (abs(sin(radians)) > 0.1) {
					ticker = 9.0;
				}
			}
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