#include "Cow.h"

Cow::Cow(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, double edge) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	SDL_SetTextureAlphaMod(sprite->texture, opacity * 255);
	speed = height / 300.0;
	Bolt::speed = height / 300.0;
	this->edge = edge;
}

void Cow::setOpacity(double o) {
	opacity = o;
	SDL_SetTextureAlphaMod(sprite->texture, opacity * 255);
}

void Cow::updateOpacity(double o) {
	opacity += o;
	SDL_SetTextureAlphaMod(sprite->texture, opacity * 255);
}

bool Cow::wiggle(double frame) {
	bool result = false;
	opacity += frame / 1000.0;
	if (opacity < 0) {
		opacity = 0;
		result = true;
	}
	else if (opacity > 1) {
		opacity = 1;
		result = true;
	}
	SDL_SetTextureAlphaMod(sprite->texture, opacity * 255);
	y += frame / 1000.0 * (float) height * cos(TAU * opacity);
	return result;
}

void Cow::prepare() {
	Enemy::prepare();
	x = gameWidth / 2;
	y = gameHeight / 2;
	phase = 0;
	boltsClear();
	angle = 0;
	flip = 0;
	setSprite(0);
	setOpacity(1);
}

void Cow::update(double frame) {
	flip = 0;
	if (phase == 1) {
		angle = 0;
		switch (behavior) {
		case 0:
			if (ticker == 0.0) {
				if (wiggle(-frame)) {
					ticker++;
				}
			}
			else if (ticker == 1.0) {
				x = rand() % (upperX - lowerX) + lowerX;
				y = rand() % (upperY - lowerY) + lowerY;
				ticker++;
			}
			else {
				if (wiggle(frame)) {
					Bolt* b = new Bolt("assets/flame.png", sprite->renderer, height / 3.0, 1, sword, sheep, NULL, this);
					b->x = x;
					b->y = y;
					b->lookAt(sheep);
					b->setBounds(edge);
					bolts.push_back(b);
					ticker = 0.0;
				}
			}
		}
	}
	else {
		angle = 15;
		posBar();
		switch (behavior) {
		case 0:
			if (ticker == 0.0) {
				if (wiggle(-frame * (2.0 - (float) health / (float) maxHealth2))) {
					ticker++;
				}
			}
			else if (ticker == 1.0) {
				int a = rand();
				x = sheep->x + height * 3 * cos(a);
				y = sheep->y + height * 3 * sin(a);
				ticker++;
			}
			else {
				if (wiggle(frame * (2.0 - (float) health / (float) maxHealth2))) {
					Bolt* b = new Bolt("assets/flame.png", sprite->renderer, height / 3.0, 1, sword, sheep, NULL, this);
					b->x = x;
					b->y = y;
					b->lookAt(sheep);
					b->setBounds(edge);
					bolts.push_back(b);
					ticker = 0.0;
				}
			}
		}
	}
	uint8_t size = bolts.size();
	for (int i = 0; i < size; i++) {
		if (bolts[i]->update(frame)) {
			bolts[i]->clear();
			bolts.erase(std::next(bolts.begin(), i));
			i--;
			size--;
		}
	}
}

void Cow::render() {
	Enemy::render();
	for (auto b : bolts) {
		b->render();
	}
	if (phase == 3) {
		bar->render();
	}
}

void Cow::boltsClear() {
	for (auto b : bolts) {
		b->clear();
	}
	bolts.clear();
}

Cow::Bolt::Bolt(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar, Cow* cow) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	immunityFrames = 1000;
	this->cow = cow;
}

bool Cow::Bolt::update(double frame) {
	if (cow->phase == 1) {
		if (shift(frame * speed)) {
			return true;
		}
	}
	else {
		slide(frame * speed);
		if (abs(x - sheep->x) > gameWidth || abs(y - sheep->y) > gameHeight) {
			return true;
		}
	}
	if (damage(cow->phase * 10)) {
		return true;
	}
	if (ticker > 0.5) {
		if (damage(100, cow)) {
			return true;
		}
	}
	else {
		ticker += frame / 1000.0;
	}
	if (immunity == 0) {
		if (collided(sword) && sword->swordOut) {
			angle += 180;
			angle = angle > 360 ? angle - 360 : angle;
			immunity = immunityFrames;
		}
	}
	else {
		immunity -= frame;
		immunity = immunity < 0 ? 0 : immunity;
	}
	return false;
}

double Cow::Bolt::speed;
