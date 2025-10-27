#include "App.h"
#include "constants.h"

void
App::setup()
{
	ball = {20, 20, 20, 20, 100, 100};
}

void
App::handle_input()
{
	if(sdl_instance.is_key_r_pressed()) {
		ball.x = 0;
		ball.y = 0;
		ball.vx = 100;
		ball.vy = 100;
	}
	if(sdl_instance.is_escape_pressed()) {
		running = false;
	}
}

bool
App::is_running() const
{
	return running;
}

void
App::update(float dt)
{
	// Update ball position
	ball.x += ball.vx * dt;
	ball.y += ball.vy * dt;
	// Contrain ball to window bounds
	if(ball.x < 0) {
		ball.x = 0;
		ball.vx = -ball.vx;
	}
	if(ball.y < 0) {
		ball.y = 0;
		ball.vy = -ball.vy;
	}
	if(ball.x + ball.w > WINDOW_WIDTH) {
		ball.x = WINDOW_WIDTH - ball.w;
		ball.vx = -ball.vx;
	}
	if(ball.y + ball.h > WINDOW_HEIGHT) {
		ball.y = WINDOW_HEIGHT - ball.h;
		ball.vy = -ball.vy;
	}
}

void
App::render()
{
	sdl_instance.fill_rect((int)ball.x, (int)ball.y, (int)ball.w, (int)ball.h, 255, 255, 255, 255);
}
