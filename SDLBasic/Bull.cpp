#include "Bull.h"

Bull::Bull(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep) :
	Enemy(spriteFile, renderer, height, health, sword, sheep) {
	this->cAngle = 0.0;
	this->cStep = 0.0;
}

bool Bull::move(double vel) {
	return shift(vel * speed);
}

void Bull::update(double frame) {
	if (cStep == 0) {
		angle = atan2(y - sheep->y, sheep->x - x) * 180.0 / PI;
		if (angle < 0) {
			angle += 360.0;
		}
		if (90 < angle && angle < 270) {
			flip = 2;
		}
		else {
			flip = 0;
		}
		cAngle = angle;
		cStep += 0.001;
	}
	else if (cStep < TAU) {
		angle = cAngle + prepAngle * sin(cStep);
		cStep += TAU * frame / chargeTime;
		cStep = cStep > TAU ? 7 : cStep;
	}
	else if (cStep == 7){
		cStep = 8;
		angle = cAngle;
		setRadians();
		move(frame);
	}
	else if (cStep == 8) {
		if (move(frame)) {
			cStep == 9;
		}
	}
}