#pragma once
#include "GameObject.h"
class TextArea :
    public GameObject
{
public:
	//Creates sprite text area.
	//font: font file
	//size: size of text
	//fg: foreground color
	//bg: background color
	//wrapLength: length to wrap text at
	//renderer: what sprite will be rendererd to
	//height: height of object; width will be based on porportions of original sprite
	TextArea(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height);
	//Creates sprite text area.
	//font: font file
	//size: size of text
	//fg: foreground color
	//wrapLength: length to wrap text at
	//renderer: what sprite will be rendererd to
	//height: height of object; width will be based on porportions of original sprite
	TextArea(std::string text, std::string font, int size, SDL_Color fg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height);
	~TextArea();
	void changeText(std::string);
protected:
	void retext();
	std::string text;
	std::string font;
	int size;
	SDL_Color fg, bg;
	Uint32 wrapLength;
	bool background;
};

