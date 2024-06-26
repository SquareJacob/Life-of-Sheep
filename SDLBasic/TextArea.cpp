#include "TextArea.h"
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define new DEBUG_NEW
#endif  

TextArea::TextArea(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height):
	GameObject(text, font, size, fg, bg, wrapLength, renderer, height) {
	this->fg = fg;
	this->bg = bg;
	this->size = size;
	this->text = text;
	this->wrapLength = wrapLength;
	this->font = "assets/" + font + ".otf";
	background = true;
}

TextArea::TextArea(std::string text, std::string font, int size, SDL_Color fg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height) :
	GameObject(text, font, size, fg, wrapLength, renderer, height) {
	this->fg = fg;
	this->size = size;
	this->text = text;
	this->wrapLength = wrapLength;
	this->font = "assets/" + font + ".otf";
	background = false;
}

TextArea::~TextArea() {}

void TextArea::retext() {
	SDL_DestroyTexture(sprite->texture);
	if (background) {
		sprite->textualize(text, font, size, fg, bg, wrapLength);
	}
	else {
		sprite->textualize(text, font, size, fg, wrapLength);
	}
	width = sprite->getPorportion() * height;
}

void TextArea::changeText(std::string text) {
	this->text = text;
	retext();
}