#include "Enemy.h"

Enemy::Enemy(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	GameObject(spriteFile, renderer, height) {
	this->health = health;
	maxHealth = health;
	this->sword = sword;
	this->sheep = sheep;
	this->bar = bar;
}

Enemy::~Enemy() {}


void Enemy::damage(int dmg) {
	if (collided(sheep)) {
		if (sheep->takeDmg(dmg)) {
			double deltaX = sheep->x - x;
			double deltaY = sheep->y - y;
			double length = sqrt(deltaX * deltaX + deltaY * deltaY);
			sheep->x += deltaX * knockback / length;
			sheep->y += deltaY * knockback / length;
			sheep->bound();
		}
	}
}

bool Enemy::damaged(bool swordOut) {
	if (collided(sword) && swordOut) {
		if (!beenDamaged) {
			health -= sword->damage();
			health = health < 0 ? 0 : health;
			bar->setValue(health);
			if (health == 0) {
				ticker = 0;
			}
			SDL_SetTextureColorMod(sprite->texture, 1, 1, 1);
			beenDamaged = true;
			return health <= 0;
		}
	}
	else {
		beenDamaged = false;
	}
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
	bar->setMax(health);
	bar->setValue(health);
}
