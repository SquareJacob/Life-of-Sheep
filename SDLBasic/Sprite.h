#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
//This class is mostly used in other classes, specifically GameObject
class Sprite
{
public:
	//Creates sprite from file.
	//file: path to get sprite from
	//renderer: what sprite will be rendererd to
	Sprite(const char* file, SDL_Renderer* renderer);
	//Creates sprite text area.
	//font: font file
	//size: size of text
	//fg: foreground color
	//bg: background color
	//wrapLength: length to wrap text at
	//renderer: what sprite will be rendererd to
	Sprite(const char* text, const char* font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer);
	~Sprite();
	//(x,y) specifies top left corner
	//rotates counterclockwise in degrees
	//0 = no flip, 1 = horizontal, 2 = vertical
	void place(int x, int y, int width, int height, double angle, uint8_t flip, SDL_Renderer* renderer);
	//width/height
	float getPorportion();
	void texturize(SDL_Surface* surface, SDL_Renderer* renderer);
	SDL_Texture* texture;
private:
	float porportion; //width/height
	SDL_Rect desR;
};

