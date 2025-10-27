#include "Game.h"
#include <SDL.h>

Game::Game(SDLWrapper &sdl_ref) : sdl(sdl_ref) {}

void
Game::setup()
{
	x = 100;
	y = 100;
	vx = 100;
	vy = 80;
}

void
Game::handle_input(const SDL_Event &event)
{
	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
		setup();
	}
}

void
Game::update(float dt)
{
	x += vx * dt;
	y += vy * dt;

	if(x < 0 || x + size > 800) vx = -vx;
	if(y < 0 || y + size > 600) vy = -vy;
}

void
Game::render()
{
	sdl.draw_rect(static_cast<int>(x), static_cast<int>(y), size, size, 255, 255, 255, 255);
}
