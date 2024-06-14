#include "Chick.h"
#include <stdlib.h>

Chick::Chick(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = (float)height / 250.0;
	knock = 3.0;
	addSprite("assets/chick.png");
}

void Chick::hatch() {
	setSprite(1);
	health = maxHealth;
	bar->setValue(health);
	behavior = 0;
	hatched = true;
}

bool Chick::move(double vel) {
	bool result = shift(speed * vel * stage);
	bar->updatePos(x - width / 2, y + height / 2);
	return result;
}

void Chick::update(double frame) {
	if (!hatched) {
		if (ticker < 1) {
			ticker += frame / 5000.0;
		}
		else {
			hatch();
		}
	}
	else {
		knockback(frame);
		if (behavior == 0) {
			lookAt(sheep);
			move(frame);
			if ((sheep->x - x) * (sheep->x - x) + (sheep->y - y) * (sheep->y - y) < sheep->height * sheep->height * 1.5) {
				ticker = 0.0;
				behavior = 1;
			}
		}
		else if (behavior == 1) {
			bar->updatePos(x - width / 2, y + height / 2);
			ticker += frame / 1000.0;
			if (ticker < stage / 6.0) {
				lookAt(sheep);
			}
			if (ticker > 1.0) {
				behavior = 2;
			}
		}
		else if (behavior == 2) {
			move(frame * 3.0);
			ticker += frame / 1000.0;
			if (ticker > 2.0) {
				ticker = 0.0;
				behavior = 0;
			}
		}
		damage(10 * stage);
		if (damaged()) {
			prepare();
		}
		stage += frame / 50000.0;
		stage = stage > 4.0 ? 4.0 : stage;
	}
}

void Chick::render() {
	Enemy::render();
	if (hatched) {
		bar->render();
	}
}

void Chick::prepare() {
	alive = false;
	hatched = false;
}