#include "Squirrel.h"

Squirrel::Squirrel(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = height / 50.0;
}

void Squirrel::update(double frame) {
	if (running) {
		lookAt(sheep);
		angle += 180;
		if (angle > 360) {
			angle -= 360;
		}
		flip = 2 - flip;
		slide(frame * speed);
		if (hypot(x - sheep->x, y - sheep->y) > height * 15) {
			lookAt(sheep);
			running = false;
			ticker = 0.0;
		}
	}
	else {
		if (hypot(x - sheep->x, y - sheep->y) < height * 4) {
			ticker += frame / 750.0 / (2.0 - float(health) / float(maxHealth));
		}
		if (ticker < 16.0 * float(health) / float(maxHealth) + 4.0) {
			acornTime += frame / 1000.0;
			angle += frame / 20.0;
			Acorn* a = new Acorn("assets/acorn.png", sprite->renderer, height / 2, 1, sword, sheep, NULL, this, speed);
			a->x = x;
			a->y = y;
			a->angle = angle;
			acorns.push_back(a);
			acornTime -= 0.5;
		}
		else {
			running = true;
		}
	}

	uint8_t size = int(acorns.size());
	for (uint8_t i = 0; i < size; i++) {
		if (acorns[i]->update(frame)) {
			acorns[i]->bar->erase();
			acorns[i]->erase();
			acorns.erase(acorns.begin() + i);
			i--;
			size--;
		}
	}
}

void Squirrel::render() {
	Enemy::render();
	for (auto a : acorns) {
		a->render();
	}
	if (phase == 1) {
		posBar();
		bar->render();
	}
}

void Squirrel::acornsClear() {
	for (auto a : acorns) {
		a->erase();
	}
	acorns.clear();
}

void Squirrel::prepare() {
	Enemy::prepare();
	acornsClear();
}

Squirrel::Acorn::Acorn(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, Squirrel* squirrel, double speed) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	this->speed = speed;
	this->squirrel = squirrel;
}

bool Squirrel::Acorn::update(double frame) {
	slide(frame * speed);
	if (abs(x - sheep->x) > gameWidth || abs(y - sheep->y) > gameHeight) {
		return true;
	}
	if (damage(20)) {
		return true;
	}
	return false;
}

uint8_t Squirrel::phase = 0;