#include "GameObject.h"
#include "Sprite.h"

void GameObject::objectize(SDL_Renderer* renderer, uint16_t height) {
	this->renderer = renderer;
	angle = 0.0;
	radians = 0.0;
	flip = 0;
	width = sprite->getPorportion() * height;
	this->height = height;
	x = width * 0.5;
	y = height * 0.5;
}

GameObject::GameObject(const char* spriteFile, SDL_Renderer* renderer, uint16_t height) {
	sprite = new Sprite(spriteFile, renderer);
	objectize(renderer, height);
}

GameObject::GameObject(const char* text, const char* font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height) {
	sprite = new Sprite(text, font, size, fg, bg, wrapLength, renderer);
	objectize(renderer, height);
}

GameObject::~GameObject() {}

void GameObject::render() {
	sprite->place(x - width / 2, y - height / 2, width, height, angle, flip, renderer);
}

double GameObject::setRadians() {
	radians = angle * PI / 180.0;
	return radians;
}
