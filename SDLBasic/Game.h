#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <set>
class Game
{
public:
	Game(const char* title, bool fullscreen);
	~Game();
	void handleEvents();
	void update(int frame);
	void render();
	void clean();
	bool running();
	SDL_Renderer* getRenderer();
	int getWidth();
	int getHeight();
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width;
	int height;
	SDL_Event event;
	std::set<std::string> keys;
};

