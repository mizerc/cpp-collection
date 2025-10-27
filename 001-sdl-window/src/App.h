#pragma once
#include "SDLWrapper.h"

struct Ball {
	int x, y, w, h, vx, vy;
};

class App {
  private:
	Ball ball;
	SDLWrap &sdl_instance;
	bool running = true;

  public:
	App(SDLWrap &input) : sdl_instance(input) {}
	void setup();
	void handle_input();
	bool is_running() const;
	void update(float dt);
	void render();
};