#include "Enemy.h"

Enemy::Enemy(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	GameObject(spriteFile, renderer, height) {
	this->health = health;
	maxHealth = health;
	this->sword = sword;
	this->sheep = sheep;
	this->bar = bar;
	text = "";
	SDL_Color black = { 0, 0, 0 };
	textArea = new TextArea(text, "BlackRunters", 250, black, 10000, renderer, 30);
}

Enemy::~Enemy() {}


bool Enemy::damage(int dmg, Enemy* object) {
	if (object == NULL) {
		if (collided(sheep)) {
			if (sheep->takeDmg(dmg)) {
				double deltaX = sheep->x - x;
				double deltaY = sheep->y - y;
				double length = sqrt(deltaX * deltaX + deltaY * deltaY);
				sheep->setKnockback(deltaX * (float)sheep->height / length * 10.0, deltaY * (float)sheep->height / length * 10.0, 100);
				return true;
			}
		}
	}
	else {
		if (collided(object)) {
			object->health -= dmg;
			object->bar->setValue(object->health);
			return true;
		}
	}
	return false;
}

bool Enemy::damaged(int frame) {
	if (immunity == 0 || (sword->poking() && !poked)) {
		if (collided(sword) && sword->swordOut) {
			health -= sword->damage();
			health = health < 0 ? 0 : health;
			bar->setValue(health);
			hits++;
			if (health == 0) {
				ticker = 0;
			}
			SDL_SetTextureColorMod(sprite->texture, 1, 1, 1);
			poked = true;
			double deltaX = x - sheep->x;
			double deltaY = y - sheep->y;
			double length = sqrt(deltaX * deltaX + deltaY * deltaY);
			setKnockback(deltaX * (float)sheep->height / length * 5.0 * knock, deltaY * (float)sheep->height / length * 5.0 * knock, 50);
			immunity = immunityFrames;
			return health <= 0;
		}
	}
	else if (!sword->poking()) {
		poked = false;
	}
	immunity -= frame;
	immunity = immunity < 0 ? 0 : immunity;
	SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
	return health <= 0;
}

bool Enemy::defeated(double frame) {
	angle += frame;
	ticker += frame;
	if (ticker >= 2000) {
		return true;
	}
	return false;
}

void Enemy::prepare() {
	health = maxHealth;
	bar->setMax(maxHealth);
	bar->setValue(health);
	setKnockback(0.0, 0.0, 0.0);
	hits = 0;
}

void Enemy::render() {
	GameObject::render();
	if (text.compare("") != 0) {
		textArea->changeText(text);
		textArea->render();
		text = "";
	}
}

void Enemy::speak(std::string text) {
	this->text = text;
	textArea->x = x;
	textArea->y = y + (textArea->height + height) / 2;
}

void Enemy::posBar() {
	bar->updatePos(x - width / 2 - GameObject::globalX, y + height / 2 - GameObject::globalY);
}
