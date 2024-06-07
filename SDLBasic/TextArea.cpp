#include "TextArea.h"

TextArea::TextArea(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height):
	GameObject(text, font, size, fg, bg, wrapLength, renderer, height) {
	this->fg = fg;
	this->bg = bg;
	this->size = size;
	this->text = text;
	this->wrapLength = wrapLength;
	this->font = "assets/" + font + ".otf";
}

TextArea::~TextArea() {}

void TextArea::retext() {
	SDL_DestroyTexture(sprite->texture);
	sprite->textualize(text, font, size, fg, bg, wrapLength);
	width = sprite->getPorportion() * height;
}

void TextArea::changeText(std::string text) {
	this->text = text;
	retext();
}