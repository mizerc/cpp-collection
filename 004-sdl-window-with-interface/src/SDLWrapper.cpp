#include "SDLWrapper.h"
#include "IGame.h"
#include <stdexcept>

SDLWrapper::SDLWrapper(int width, int height)
	: width(width), height(height)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error("SDL_Init failed");

	window = SDL_CreateWindow("Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN);
	if(!window) {
		throw std::runtime_error("SDL_CreateWindow failed");
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) {
		throw std::runtime_error("SDL_CreateRenderer failed");
	}

	last_ticks = SDL_GetTicks();
}

SDLWrapper::~SDLWrapper()
{
	if(renderer) SDL_DestroyRenderer(renderer);
	if(window) SDL_DestroyWindow(window);
	SDL_Quit();
}

void
SDLWrapper::run(IGame *game)
{
	game->setup();

	while(running) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT)
				running = false;
			else
				game->handle_input(e);
		}

		int now = SDL_GetTicks();
		delta_time = (now - last_ticks) / 1000.0f;
		last_ticks = now;

		game->update(delta_time);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		game->render();

		SDL_RenderPresent(renderer);
	}
}

void
SDLWrapper::draw_rect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderFillRect(renderer, &rect);
}
