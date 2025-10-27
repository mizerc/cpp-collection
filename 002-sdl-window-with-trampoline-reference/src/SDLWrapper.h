#pragma once

#include <SDL.h>

#define SDLWRAP_KEYDOWN SDL_KEYDOWN
#define SDLWRAP_KEYDOWN_ESC SDLK_ESCAPE
#define SDLWRAP_KEYDOWN_R SDLK_r

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

class SDLWrap {
  public:
	SDLWrap(int width, int height);

	int run(
		void (*setup)(),
		void (*input)(SDL_Event),
		void (*update)(float),
		void (*render)());

	void fill_rect(int x, int y, int width, int height, int r, int g, int b, int a) const;
	SDL_Renderer *get_renderer() const;

  private:
	int W, H;
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool game_is_running;
	int last_frame_time;
	float delta_time;

	void (*ptr_setup)();
	void (*ptr_input)(SDL_Event);
	void (*ptr_update)(float);
	void (*ptr_render)();

	bool initialize_window();
	void destroy_window();
	void process_input();
	void update_time();
	void clear() const;
	void present() const;
};
