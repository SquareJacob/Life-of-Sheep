#include "Sprite.h"

Sprite::Sprite(const char* file, SDL_Renderer* renderer) {
	SDL_Surface* tmpSurface = IMG_Load(file);
	if (tmpSurface == NULL) {
		std::cout << SDL_GetError() << std::endl;
		porportion = 1.0;
		texture = NULL;
		return;
	}
	porportion = (float)tmpSurface->w / (float)tmpSurface->h;
	texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if (texture == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return;
	}
}

Sprite::~Sprite() {}

void Sprite::place(int x, int y, int width, int height, double angle, uint8_t flip, SDL_Renderer* renderer) {
	desR.x = x;
	desR.y = y;
	desR.w = width;
	desR.h = height;
	SDL_RendererFlip f;
	switch (flip) {
	case 1:
		f = SDL_FLIP_HORIZONTAL;
		break;
	case 2:
		f = SDL_FLIP_VERTICAL;
		break;
	default:
		f = SDL_FLIP_NONE;
		break;
	}
	SDL_RenderCopyEx(renderer, texture, NULL, &desR, -angle, NULL, f);
}

float Sprite::getPorportion() {
	return porportion;
}

