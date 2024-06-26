#pragma once
#include "Sprite.h"
#include <math.h>
#include <string>
#include <cmath>
#include <vector>
#include <set>
#include <stdlib.h>
#include <stdlib.h>  
#include <crtdbg.h>   //for malloc and free
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define DEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW
#endif
#define PI 3.14159265
#define QPI PI/4.0 //Quarter of PI
#define TAU PI * 2
#define  red SDL_Color{ 191, 33, 47 }
#define black SDL_Color{ 0, 0, 0 }
#define green SDL_Color{ 0, 111, 60 }
#define blue SDL_Color{ 37, 75, 150 }
#define yellow SDL_Color{ 250, 167, 63 }
#define white SDL_Color{ 255, 255, 255 }
#define big boy
//Should be subclassed into specific objects
class GameObject
{
public:
	//destroys surface upon finish
	GameObject(SDL_Surface* surface, SDL_Renderer* renderer, uint16_t height);
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
	//Creates sprite text area.
	//font: font file
	//size: size of text
	//fg: foreground color
	//wrapLength: length to wrap text at
	//renderer: what sprite will be rendererd to
	//height: height of object; width will be based on porportions of original sprite
	GameObject(std::string text, std::string font, int size, SDL_Color fg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height);
	virtual ~GameObject() { }
	//renders the object, centered at (x,y)
	void render();
	//sets public field radians using angle, and returns radians
	double setRadians();
	//Moves object in direction it is facing, or opposite if vel < 0. vel porpotional to change in position, returns if it hit an edge
	bool shift(double vel);
	//Moves object in direction it is facing, or opposite if vel < 0. vel porportional to change in position. Ignores edges
	void slide(double vel);
	//Rotates object amount porportional to vel. ccw is positive. Sets radians
	void tilt(double vel);
	//setup local edge collision bounds
	void setBounds();
	void setBounds(double edge);
	//places sheep returned in bounds if needs, returns if sheep was moved
	bool bound();
	//put all corners into a vector; even is x, odd is y, goes in order of quadrant before rotation and flip
	void cornerize(std::vector<double>& corners);
	void renderHitBox(int r, int g, int b);
	//check collision
	bool collided(GameObject* object, bool talk = false);
	bool inArea(double x, double y, bool talk = false);
	void addSprite(const char* file);
	void addSprite(SDL_Surface* surface);
	void setSprite(uint8_t number);
	void setKnockback(double x, double y, double time);
	void knockback(double frame, bool bound = true);
	void clear();
	void erase();
	void lookAt(GameObject* object);
	//0 = invisible, 1 = visible; changes only current sprite
	void setOpacity(double o);
	void turnTowards(GameObject* object, double amount);
	int16_t width;
	int16_t height;
	int16_t cWidth, cHeight;
	double x; //center xpos
	double y; //center ypos
	double angle; //degrees
	double radians;
	uint8_t flip; //0 = no flip, 1 = horizontal, 2 = vertical
	static std::vector<GameObject*> objects;
	static double globalX, globalY;
protected:
	//prep function used in constructors
	void objectize(uint16_t height);
	Sprite* sprite;
	std::vector<Sprite*> sprites;
	int gameWidth = 0, gameHeight = 0;
	int16_t lowerX, lowerY, upperX, upperY;
	double knockX = 0.0;
	double knockY = 0.0;
	double knockTime = 0.0;
private:
	static double c, s;
	static std::vector<double> corners; //even is x, odd is y, goes in order of quadrant before rotation and flip
	static std::vector<double> uVectors; //used in collided; this.x, this.y, object.x, object.y
	static std::vector<double> distances; //used in collided; holds the 4 distances for the corners
};

