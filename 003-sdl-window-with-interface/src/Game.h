#pragma once
#include "IGame.h"
#include "SDLWrapper.h"

class Game : public IGame {
  public:
	Game(SDLWrapper &sdl);
	void setup() override;
	void handle_input(const SDL_Event &event) override;
	void update(float dt) override;
	void render() override;

  private:
	SDLWrapper &sdl;
	float x = 100, y = 100;
	float vx = 100, vy = 80;
	int size = 20;
};
