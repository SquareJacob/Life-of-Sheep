#include "GameObject.h"
#include "Sprite.h"

GameObject::GameObject(const char* spriteFile, SDL_Renderer* renderer, uint16_t height) {
	this->renderer = renderer;
	sprite = new Sprite(spriteFile, renderer);
	angle = 0.0;
	radians = 0.0;
	flip = 0;
	width = sprite->getPorportion() * height;
	this->height = height;
	x = width * 0.5;
	y = height * 0.5;
}

GameObject::~GameObject() {}

void GameObject::render() {
	sprite->place(x - width / 2, y - height / 2, width, height, angle, flip, renderer);
}

double GameObject::setRadians() {
	radians = angle * PI / 180.0;
	return radians;
}
