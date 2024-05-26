#pragma once
#include "Sprite.h"
class GameObject
{
public:
	GameObject(const char* spriteFile, SDL_Renderer* renderer, uint16_t height);
	~GameObject();
	void render();
	int16_t x, y, width, height;
	double angle;
	uint8_t flip; //0 = no flip, 1 = horizontal, 2 = vertical
private:
	Sprite* sprite;
	SDL_Renderer* renderer;
};

