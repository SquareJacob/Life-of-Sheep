#include "GameObject.h"
#include "Sprite.h"

GameObject::GameObject(const char* spriteFile, SDL_Renderer* renderer, uint16_t height) {
	this->renderer = renderer;
	sprite = new Sprite(spriteFile, renderer);
	x = 0;
	y = 0;
	angle = 0.0;
	flip = 0;
	width = sprite->getPorportion() * height;
	this->height = height;
}

GameObject::~GameObject() {}

void GameObject::render() {
	sprite->place(x, y, width, height, angle, flip, renderer);
}
