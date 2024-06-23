#include "Button.h"

Button::Button(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height, int* mX, int* mY, std::set<int>* buttons, std::set<int>* cButtons) :
	TextArea(text, font, size, fg, bg, wrapLength, renderer, height) {
	mouseX = mX;
	mouseY = mY;
	this->buttons = buttons;
	currentButtons = cButtons;
}

bool Button::hovered() {
	return inArea(*mouseX, *mouseY);
}

bool Button::clicked() {
	return hovered() && currentButtons->contains(1);
}

void Button::render() {
	if (hovered()) {
		if (buttons->contains(1)) {
			SDL_SetTextureColorMod(sprite->texture, 1, 1, 1);
		}
		else {
			SDL_SetTextureAlphaMod(sprite->texture, 127);
			SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
		}
	}
	GameObject::render();
	SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
	SDL_SetTextureAlphaMod(sprite->texture, 255);
}