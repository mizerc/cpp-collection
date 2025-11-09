#include <stdlib.h>
#include <iostream>
#include <array>
#include <SDL.h>

const int WINDOW_W = 800;
const int WINDOW_H = 600;

int main(int argc, char **argv)
{
	std::cout << "hi\n";

	// Create SDL Window
	SDL_Window *window;
	window = SDL_CreateWindow("SDLWrap Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
	if(!window) {
		SDL_Log("Error creating SDL Window: %s", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// Create SDL 2D renderer context
	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer) {
		SDL_Log("Error creating SDL Renderer: %s", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// Create input state (scancode to use as index)
	std::array<bool, SDL_NUM_SCANCODES> keyDown{};

	// Application state
	bool quit = false;

	// Timing
	float lastFrameTimeMs = 0;

	// Object
	int x = 50;
	int y = 50;
	int w = 100;
	int h = 100;
	SDL_Rect rect = {x, y, w, h};

	const Uint8 r = 255;
	const Uint8 g = 0;
	const Uint8 b = 0;
	const Uint8 a = 255;
	SDL_Color rectColor = {r, g, b, a};

	// Main loop
	while(!quit) {
		// Update delta time
		Uint32 nowMs = SDL_GetTicks();
		float deltaTimeMs = (nowMs - lastFrameTimeMs) / 1000.0f;
		lastFrameTimeMs = nowMs;

		// Poll events from queue
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
			if(e.type == SDL_KEYDOWN) {
				keyDown[e.key.keysym.scancode] = true;
			}
			if(e.type == SDL_KEYUP) {
				keyDown[e.key.keysym.scancode] = false;
			}
		}

		// Process input
		if(keyDown[SDL_SCANCODE_ESCAPE]) {
			quit = true;
		}

		// Update object color based on frame time
		if(deltaTimeMs > 16) {
			rectColor.r = 255;
			rectColor.g = 0;
			rectColor.b = 0;
		} else {
			rectColor.r = 0;
			rectColor.g = 255;
			rectColor.b = 0;
		}

		// Clear
		SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
		SDL_RenderClear(renderer);

		// Render rect
		SDL_SetRenderDrawColor(renderer, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
		SDL_RenderFillRect(renderer, &rect);

		// Present
		SDL_RenderPresent(renderer);

		// Delay to cap frame rate
		Uint32 delayDurationMs = 16; // Approx ~60 FPS
		SDL_Delay(delayDurationMs);
	}

	// Cleanup and quit SDL
	if(renderer) SDL_DestroyRenderer(renderer);
	if(window) SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "bye\n";
	return EXIT_SUCCESS;
}
