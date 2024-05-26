#include "Game.h"
#include <filesystem>
namespace fs = std::filesystem;


Game* game = nullptr;
int main(int argc, char* argv[]) {
	std::cout << "Current path is " << fs::current_path() << '\n';
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