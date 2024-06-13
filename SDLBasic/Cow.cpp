#include "Cow.h"

Cow::Cow(const char* spriteFile, SDL_Renderer* renderer, uint16_t height, int health, Sword* sword, Sheep* sheep, Bar* bar) :
	Enemy(spriteFile, renderer, height, health, sword, sheep, bar) {
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
