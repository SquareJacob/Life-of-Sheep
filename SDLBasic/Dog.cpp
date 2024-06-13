#include "Dog.h"

Dog::Dog(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = height / 200.0;
	aSpeed = baseAngleSpeed;
	knock = 1.0;
}

bool Dog::move(double vel) {
	return shift(vel * speed);
}

void Dog::prepare() {
	Enemy::prepare();
	x = upperX;
	y = upperY;
	ticker = 0;
}

void Dog::update(double frame) {
	//regen 4ph/s
	if (health < maxHealth) {
		healthFraction += frame / 250.0;
		if (healthFraction >= 1) {
			health++;
			healthFraction -= 1.0;
			bar->setValue(health);
		}
	}
	else {
		healthFraction = 0.0;
	}


	if (behavior == 0) {
		if (ticker == 0) { //turn to sheep
			lookAt(sheep);
			ticker += frame / 1000;
		}
		else { //run towards sheep
			aSpeed = baseAngleSpeed * (3.0 - 2.0 * (float)health / (float)maxHealth);
			sAngle = atan2(y - sheep->y, sheep->x - x) * 180.0 / PI;
			if (sAngle < 0) {
				sAngle += 360.0;
			}
			if (sAngle < angle) {
				if (angle - sAngle < 180.0) {
					angle -= frame * aSpeed;
				}
				else {
					angle += frame * aSpeed;
					if (angle > 360) {
						angle -= 360;
					}
				}
			}
			else {
				if (sAngle - angle < 180.0) {
					angle += frame * aSpeed;
				}
				else {
					angle -= frame * aSpeed;
					if (angle < 0) {
						angle += 360;
					}
				}
			}
			if (move(frame)) {
				ticker = 0;
			}
			damage(15);
		}
	}
	else if (behavior == 1) {
		lookAt(sheep);
		damage(1);
		ticker += frame / 3000.0;
		if (ticker > 1.0) {
			ticker = 0.0;
			behavior = 0;
		}
	}
}

bool Dog::damaged() {
	bool result = Enemy::damaged();
	if (behavior == 1) {
		hits = realHits;
	}
	else if (behavior == 0 && realHits != hits) {
		staggered = false;
		realHits = hits;
		flip = 2 - flip;
		angle += 180;
		if (angle > 360) {
			angle -= 360;
		}
	}
	if (hits % 7 == 0 && hits != 0 && !staggered) {
		behavior = 1;
		ticker = 0;
		staggered = true;
	}
	return result;
}
