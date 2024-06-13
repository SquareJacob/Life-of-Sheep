#include "Bar.h"
#include <string>

Bar::Bar(Uint32 iValue, Uint32 mValue, SDL_Color fg, SDL_Color bg, Uint16 x, Uint16 y, Uint16 length, bool vertical, SDL_Renderer* renderer) {
	this->iValue = iValue;
	this->mValue = mValue;
	fColor = fg;
	bColor = bg;
	this->length = length;
	this->vertical = vertical;
	this->x = x;
	this->y = y;
	this->renderer = renderer;
	createRects();
}

void Bar::updatePos(int16_t x, int16_t y) {
	this->x = x;
	this->y = y;
	createRects();
}

void Bar::createRects() {
	bg.x = x;
	bg.y = y;
	if (vertical) {
		bg.w = height;
		bg.h = length;
	}
	else {
		bg.w = length;
		bg.h = height;
	}
	updateBar();
}

void Bar::updateBar() {
	fg.x = x;
	fg.y = y;
	if (vertical) {
		fg.w = height - height / 3;
		fg.h = length * iValue / mValue;
		fg.x += height / 6;
		fg.y += length - length * iValue / mValue;
	}
	else {
		fg.w = length * iValue / mValue;
		fg.h = height - height / 3;
		fg.y += height / 6;
	}
}

void Bar::setValue(Uint32 value) {
	iValue = value;
	updateBar();
}

void Bar::render() {
	//std::cout << std::to_string(iValue) + "/" + std::to_string(mValue) << std::endl;
	SDL_SetRenderDrawColor(renderer, bColor.r, bColor.g, bColor.b, 255);
	SDL_RenderFillRect(renderer, &bg);
	SDL_SetRenderDrawColor(renderer, fColor.r, fColor.g, fColor.b, 255);
	SDL_RenderFillRect(renderer, &fg);
}

void Bar::setMax(Uint32 value) {
	mValue = value;
	updateBar();
}

void Bar::updateHeight(Uint16 height) {
	this->height = height;
	createRects();
}

void Bar::updateLength(Uint16 length) {
	this->length = length;
	createRects();
}

Uint16 Bar::getHeight() {
	return height;
}