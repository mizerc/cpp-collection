// SDLWrapper.h
#pragma once
#include <SDL.h>

class IGame;

class SDLWrapper {
  public:
	SDLWrapper(int width, int height);
	~SDLWrapper();

	void run(IGame *game);

	void draw_rect(int x, int y, int w, int h, int r, int g, int b, int a);

  private:
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	int width, height;
	bool running = true;
	float delta_time = 0.0f;
	int last_ticks = 0;
};
