#include "SDLWrapper.h"

SDLWrap::SDLWrap(int width, int height)
	: W(width), H(height), window(nullptr), renderer(nullptr),
	  game_is_running(false), last_frame_time(0), delta_time(0),
	  ptr_setup(nullptr), ptr_input(nullptr), ptr_update(nullptr), ptr_render(nullptr)
{
}

int
SDLWrap::run(
	void (*setup)(),
	void (*input)(SDL_Event),
	void (*update)(float),
	void (*render)())
{
	ptr_setup = setup;
	ptr_input = input;
	ptr_update = update;
	ptr_render = render;

	game_is_running = initialize_window();
	if(!game_is_running) return -1;

	ptr_setup();

	while(game_is_running) {
		process_input();
		update_time();
		ptr_update(delta_time);
		clear();
		ptr_render();
		present();
	}

	destroy_window();
	return 0;
}

bool
SDLWrap::initialize_window()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Error initializing SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("SDLWrap Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
	if(!window) {
		SDL_Log("Error creating SDL Window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer) {
		SDL_Log("Error creating SDL Renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void
SDLWrap::destroy_window()
{
	if(renderer) SDL_DestroyRenderer(renderer);
	if(window) SDL_DestroyWindow(window);
	SDL_Quit();
}

void
SDLWrap::process_input()
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
			game_is_running = false;
		} else {
			ptr_input(e);
		}
	}
}

void
SDLWrap::update_time()
{
	int now = SDL_GetTicks();
	delta_time = (now - last_frame_time) / 1000.0f;
	last_frame_time = now;
}

void
SDLWrap::clear() const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void
SDLWrap::present() const
{
	SDL_RenderPresent(renderer);
}

void
SDLWrap::fill_rect(int x, int y, int width, int height, int r, int g, int b, int a) const
{
	SDL_Rect rect = {x, y, width, height};
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);
}

SDL_Renderer *
SDLWrap::get_renderer() const
{
	return renderer;
}
