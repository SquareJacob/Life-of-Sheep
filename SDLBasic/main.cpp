#include "Game.h"
#include <windows.h>
#include <string>


Game* game = nullptr;
int main(int argc, char* argv[]) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime = 0;
	game = new Game("Window", false);
	while (game->running()) {
		frameStart = SDL_GetTicks();
		game->handleEvents();
		game->update((frameTime > frameDelay) ? frameTime : frameDelay);
		game->render();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	return 0;
}