#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
class Sprite
{
public:
	//This class is mostly used in other classes, specifically GameObject
	//file: path to get sprite from
	//renderer: what sprite will be rendererd to
	Sprite(const char* file, SDL_Renderer* renderer);
	~Sprite();
	//(x,y) specifies top left corner
	//rotates counterclockwise in degrees
	//0 = no flip, 1 = horizontal, 2 = vertical
	void place(int x, int y, int width, int height, double angle, uint8_t flip, SDL_Renderer* renderer);
	//width/height
	float getPorportion();
private:
	float porportion; //width/height
	SDL_Texture* texture;
	SDL_Rect desR;
};

