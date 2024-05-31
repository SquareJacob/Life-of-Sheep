#include "Enemy.h"

Enemy::Enemy(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep) :
	GameObject(spriteFile, renderer, height) {
	this->health = health;
	this->sword = sword;
	this->sheep = sheep;
}

Enemy::~Enemy() {}


void Enemy::damage(int dmg) {

}

void Enemy::damaged(bool swordOut) {
	if (collided(sword) && swordOut) {
		if (!beenDamaged) {
			health -= sword->damage();
			SDL_SetTextureColorMod(sprite->texture, 1, 1, 1);
			beenDamaged = true;
			return;
		}
	}
	else {
		beenDamaged = false;
	}
	SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
	
}
