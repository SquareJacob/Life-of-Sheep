#include "Sprite.h"


void Sprite::texturize(SDL_Surface* surface, SDL_Renderer* renderer) {
	if (surface == NULL) {
		std::cout << SDL_GetError() << std::endl;
		porportion = 1.0;
		return;
	}
	porportion = (float)surface->w / (float)surface->h;
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return;
	}
}

Sprite::Sprite(const char* file, SDL_Renderer* renderer) {
	SDL_Surface* tmpSurface = IMG_Load(file);
	texturize(tmpSurface, renderer);
}

void Sprite::textualize(std::string text, std::string path, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer) {
	TTF_Font* Font = TTF_OpenFont(path.c_str(), size);
	if (Font == NULL) {
		std::cout << SDL_GetError() << std::endl;
		porportion = 1.0;
		return;
	}
	SDL_Surface* tmpSurface = TTF_RenderText_LCD_Wrapped(Font, text.c_str(), fg, bg, wrapLength);
	TTF_CloseFont(Font);
	texturize(tmpSurface, renderer);
}

Sprite::Sprite(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer) {
	std::string path = "assets/" + font + ".otf";
	textualize(text, path, size, fg, bg, wrapLength, renderer);
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

