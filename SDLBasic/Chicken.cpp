#include "Chicken.h"

Chicken::Chicken(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, double edge, SDL_Color fg, SDL_Color bg) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = (float)height / 1000.0;
	chickHealth = health / 10;
	chickCost = health / 20;
	Bar* chickBar;
	for (int i = 0; i < 3; i++) {
		chickBar = new Bar(chickHealth, chickHealth, fg, bg, 0, 0, 100, false, renderer);
		chicks.push_back(new Chick("assets/egg.png", renderer, height / 2, chickHealth, sword, sheep, chickBar));
		chickBar->updateHeight(height / 4);
		chickBar->updateLength(chicks[i]->width * 1.5);
		chicks[i]->setBounds(edge);
	}
}

bool Chicken::move(double vel) {
	return shift(speed * vel);
}

void Chicken::update(double frame) {
	//look away from sheep
	lookAt(sheep);
	angle += 180;
	flip = 2 - flip;

	//retreat, and teleport to center if at edge
	if (move(frame)) {
		x = gameWidth / 2;
		y = gameHeight / 2;
	}
	damage(5);

	//prepare egg
	eggReady += frame / 7000.0;
	if (eggReady > 1.0 && health > chickCost) {
		eggReady -= 1.0;
		for (auto c : chicks) {
			if (!c->alive) {
				c->alive = true;
				c->x = x;
				c->y = y;
				c->flip = 0;
				c->angle = 0;
				c->setSprite(0);
				c->ticker = 0;
				c->setKnockback(0.0, 0.0, 0.0);
				health -= chickCost;
				bar->setValue(health);
				c->stage = 1.0 - (float)health / float(maxHealth);
				break;
			}
		}
	}

	//update chicks, and stop egg production if all chicks out
	alive = 0;
	for (auto c : chicks) {
		if (c->alive) {
			c->update(frame);
			alive++;
		}
	}
	if (alive == 3) {
		eggReady = 0.0;
	}
}

void Chicken::prepare() {
	Enemy::prepare();
	x = gameWidth / 2;
	y = gameHeight / 2;
	ticker = 0.0;
	eggReady = 0.0;
	for (auto c : chicks) {
		c->prepare();
	}
}

void Chicken::render() {
	Enemy::render();
	for (auto c : chicks) {
		if (c->alive) {
			c->render();
		}
	}
}