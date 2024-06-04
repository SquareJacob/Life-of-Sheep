#pragma once
#include "Sprite.h"
#include <math.h>
#include <cmath>
#include <vector>
#define PI 3.14159265
#define QPI PI/4.0 //Quarter of PI
//Should be subclasses into specific objects unless using for text
class GameObject
{
public:
	//Places object in topleft corner intially.
	//spriteFile: path to get sprite from
	//renderer: what GameObject will be rendererd to
	//height: height of object; width will be based on porportions of original sprite
	GameObject(const char* spriteFile, SDL_Renderer* renderer, uint16_t height);
	//Creates sprite text area.
	//font: font file
	//size: size of text
	//fg: foreground color
	//bg: background color
	//wrapLength: length to wrap text at
	//renderer: what sprite will be rendererd to
	//height: height of object; width will be based on porportions of original sprite
	GameObject(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height);
	~GameObject();
	//renders the object, centered at (x,y)
	void render();
	//sets public field radians using angle, and returns radians
	double setRadians();
	//Moves object in direction it is facing, or opposite if vel < 0. vel porpotional to change in position, returns if it hit an edge
	bool shift(double vel);
	//Rotates object amount porportional to vel. ccw is positive. Sets radians
	void tilt(double vel);
	//setup local edge collision bounds
	void bound();
	//put all corners into a vector; even is x, odd is y, goes in order of quadrant before rotation and flip
	void cornerize(std::vector<double>& corners);
	void renderHitBox(int r, int g, int b);
	//check collision
	bool collided(GameObject* object, bool talk = false);
	int16_t width;
	int16_t height;
	double x; //center xpos
	double y; //center ypos
	double angle; //degrees
	double radians;
	uint8_t flip; //0 = no flip, 1 = horizontal, 2 = vertical
protected:
	//prep function used in constructors
	void objectize(SDL_Renderer* renderer, uint16_t height);
	Sprite* sprite;
	SDL_Renderer* renderer;
	int gameWidth = 0, gameHeight = 0;
	int16_t lowerX, lowerY, upperX, upperY;
private:
	static double c, s;
	static std::vector<double> corners; //even is x, odd is y, goes in order of quadrant before rotation and flip
	static std::vector<double> uVectors; //used in collided; this.x, this.y, object.x, object.y
	static std::vector<double> distances; //used in collided; holds the 4 distances for the corners
};

