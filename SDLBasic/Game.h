#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <set>
#include "Hand.h"
class Game
{
public:
	//creates a window the size of the display screen
	//title: title of window
	//fullscreen: whether to fullscreen
	Game(const char* title, bool fullscreen);
	~Game();
	//handles inputs
	void handleEvents();
	//updates objects based on time frame took to keep things looking constant despite varying framerate
	void update(int frame);
	//renders objects
	void render();
	//delets and cleans everything
	void clean();
	bool running();
	SDL_Renderer* getRenderer();
	int getWidth();
	int getHeight();
	//tries to update gold, without going negative, returns if successfully updated gold
	bool updateGold(int amount);
	Mix_Music* createMusic(const char* file);
	//sends thee to the prepare room
	void prepare();
	//sends thee to the next level
	void levelup();
	void farmerSpeak(std::string text);
	Hand* createHand(uint16_t height, int health);
	void destroyHand(Hand* hand);
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width;  //width of window
	int height; //height of window
	//porpotion of width and height meant for battle
	double edge;
	double originalEdge = 0.95;
	//x-coord of topleft corner of battle
	double battleX;
	//y-coord of topleft corner of battle
	double battleY;
	SDL_Event event;
	std::set<std::string> keys; //set of keys currently
	std::set<std::string> currentKeys;
	std::string room; //Menu, Prepare, Level[1-10]
	int level; //current level
};

