#pragma once
#include "Sprite.h"
#include <math.h>
#include <cmath>
#define PI 3.14159265
class GameObject
{
public:
	//Places object in topleft corner intially. Should be subclasses into specific objects.
	//spriteFile: path to get sprite from
	//renderer: what GameObject will be rendererd to
	//height: height of object; width will be based on porportions of original sprite
	GameObject(const char* spriteFile, SDL_Renderer* renderer, uint16_t height);
	~GameObject();
	//renders the object, centered at (x,y)
	void render();
	//sets public field radians using angle, and returns radians
	double setRadians();
	int16_t width;
	int16_t height;
	double x; //center xpos
	double y; //center ypos
	double angle; //degrees
	double radians;
	uint8_t flip; //0 = no flip, 1 = horizontal, 2 = vertical
private:
	Sprite* sprite;
	SDL_Renderer* renderer;
};

