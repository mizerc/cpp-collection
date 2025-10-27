#pragma once

#include <SDL.h>
#include "constants.h"

#define SDLWRAP_KEYDOWN SDL_KEYDOWN
#define SDLWRAP_KEYDOWN_ESC SDLK_ESCAPE
#define SDLWRAP_KEYDOWN_R SDLK_r

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

class SDLWrap {
  private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	int last_frame_time;
	float delta_time;
	bool key_r_pressed = false;
	bool key_escape_pressed = false;

  public:
	SDLWrap();
	bool initialize_window();
	void process_input();
	void update_deltatime();
	float get_deltatime() const;
	void destroy_window();
	void clear() const;
	void fill_rect(int x, int y, int width, int height, int r, int g, int b, int a) const;
	SDL_Renderer *get_renderer() const;
	void present() const;
	bool is_key_r_pressed();
	bool is_escape_pressed();
};
