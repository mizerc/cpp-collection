#include "SDLWrapper.h"

SDLWrap::SDLWrap()
	: window(nullptr), renderer(nullptr), last_frame_time(0), delta_time(0.0f)
{
}

bool
SDLWrap::initialize_window()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Error initializing SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("SDLWrap Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
			key_escape_pressed = true;
		}
		if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
			key_r_pressed = true;
		}
	}
}

bool
SDLWrap::is_key_r_pressed()
{
	bool was_pressed = key_r_pressed;
	key_r_pressed = false; // reset after query
	return was_pressed;
}

bool
SDLWrap::is_escape_pressed()
{
	bool was_pressed = key_escape_pressed;
	key_escape_pressed = false; // reset after query
	return was_pressed;
}

void
SDLWrap::update_deltatime()
{
	int now = SDL_GetTicks();
	delta_time = (now - last_frame_time) / 1000.0f;
	last_frame_time = now;
}

float
SDLWrap::get_deltatime() const
{
	return delta_time;
}

void
SDLWrap::clear() const
{
	SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
	SDL_RenderClear(renderer);
}

SDL_Renderer *
SDLWrap::get_renderer() const
{
	return renderer;
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
