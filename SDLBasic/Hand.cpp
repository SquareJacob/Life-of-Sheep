#include "Hand.h"

Hand::Hand(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
	speed = 10.0 / (float) height;
}

bool Hand::moveTowards(GameObject* object, double frame) {
	if (collided(object) && frame > 0) {
		return true;
	}
	oAngle = atan2(y - object->y, object->x - x) * 180.0 / PI;
	if (oAngle < 0) {
		oAngle += 360.0;
	}
	if (abs(oAngle - angle) < abs(frame * aSpeed)) {
		angle = oAngle;
	}
	else if (oAngle < angle) {
		if (angle - oAngle < 180.0) {
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
		if (oAngle - angle < 180.0) {
			angle += frame * aSpeed;
		}
		else {
			angle -= frame * aSpeed;
			if (angle < 0) {
				angle += 360;
			}
		}
	}
	slide(frame * speed);
	return false;
}

double Hand::oAngle;