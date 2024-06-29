#include "Game.h"
#include <filesystem>
namespace fs = std::filesystem;
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Game* game = nullptr;
	srand(time(0));
	std::cout << "Current path is " << fs::current_path() << '\n';
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	const int maxFrame = 300;
	Uint32 frameStart;
	int frameTime = 0;
	game = new Game("Window", false);
	while (game->running()) {
		frameStart = SDL_GetTicks();
		game->handleEvents();
		game->update((frameTime > frameDelay) ? (frameTime > maxFrame ? maxFrame : frameTime) : frameDelay);
		game->render();
		frameTime = SDL_GetTicks() - frameStart;
		std::cout << frameTime << std::endl;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	delete game;
	return 0;
}