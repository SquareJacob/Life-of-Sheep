#include "Hand.h"

Hand::Hand(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = (float) (gameWidth * gameWidth) / (float) height / 250000.0;
}

bool Hand::moveTowards(GameObject* object, double frame) {
	if (collided(object) && frame > 0) {
		return true;
	}
	turnTowards(object, aSpeed * frame);
	slide(frame * speed);
	return false;
}

double Hand::oAngle;