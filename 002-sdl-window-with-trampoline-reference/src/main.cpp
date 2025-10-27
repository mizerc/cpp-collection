#include <iostream>
#include "constants.h"
#include "SDLWrapper.h"

struct GameObject {
	float x, y;
	float width, height;
	float vel_x, vel_y;
};

class GameApp {
  public:
	GameApp(SDLWrap &sdl) : sdl(sdl) {}

	void setup()
	{
		ball = {10, 20, 20, 20, 180, 140};
	}

	void handle_input(SDL_Event event)
	{
		if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_r) {
				ball.x = 0;
				ball.y = 0;
				ball.vel_x = 180;
				ball.vel_y = 140;
			}
		}
	}

	void update(float dt)
	{
		ball.x += ball.vel_x * dt;
		ball.y += ball.vel_y * dt;

		if(ball.x < 0) {
			ball.x = 0;
			ball.vel_x = -ball.vel_x;
		}
		if(ball.y < 0) {
			ball.y = 0;
			ball.vel_y = -ball.vel_y;
		}
		if(ball.x + ball.width > WINDOW_WIDTH) {
			ball.x = WINDOW_WIDTH - ball.width;
			ball.vel_x = -ball.vel_x;
		}
		if(ball.y + ball.height > WINDOW_HEIGHT) {
			ball.y = WINDOW_HEIGHT - ball.height;
			ball.vel_y = -ball.vel_y;
		}
	}

	void render()
	{
		sdl.fill_rect((int)ball.x, (int)ball.y, (int)ball.width, (int)ball.height, 255, 255, 255, 255);
	}

	// Trampoline functions for C-style SDLWrap interface
	static void trampoline_setup() { instance->setup(); }
	static void trampoline_input(SDL_Event e) { instance->handle_input(e); }
	static void trampoline_update(float dt) { instance->update(dt); }
	static void trampoline_render() { instance->render(); }

	static void bind(GameApp *inst) { instance = inst; }

  private:
	static GameApp *instance;
	GameObject ball;
	SDLWrap &sdl;
};

GameApp *GameApp::instance = nullptr;

int
main()
{
	std::cout << "hi\n";
	SDLWrap sdl(WINDOW_WIDTH, WINDOW_HEIGHT);
	GameApp game(sdl);
	GameApp::bind(&game);

	sdl.run(
		GameApp::trampoline_setup,
		GameApp::trampoline_input,
		GameApp::trampoline_update,
		GameApp::trampoline_render);

	return 0;
}
