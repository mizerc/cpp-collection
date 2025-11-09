#include <stdlib.h>
#include <iostream>
#include <array>
#include <SDL.h>
#include <SDL_ttf.h>

void renderTextAt(std::string const &text, TTF_Font *font, SDL_Renderer *renderer, int x, int y)
{
	SDL_Color color = {255, 255, 255, 255};
	SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	int textW = surface->w;
	int textH = surface->h;
	SDL_Rect dstRect = {x, y, textW, textH};
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);
}

int main(int argc, char **argv)
{
	std::cout << "hi\n";

	// Parse command-line arguments
	if(argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <ValueInt> <ValueStr>\n";
		std::exit(EXIT_FAILURE);
	}
	int valueInt = std::stoi(argv[1]);
	char const *valueStr = argv[2];
	std::cout << "ValueInt: " << valueInt << ", ValueStr: " << valueStr << "\n";

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("Error initializing SDL: %s", SDL_GetError());
		return false;
	}

	// Initialize SDL_ttf
	if(TTF_Init() == -1) {
		SDL_Log("Error initializing SDL_ttf: %s", TTF_GetError());
		return false;
	}

	// Load font
	TTF_Font *font = TTF_OpenFont("../resources/AcPlus_IBM_VGA_8x16.ttf", 18);
	if(!font) {
		printf("Failed to load font: %s\n", TTF_GetError());
	}

	// Create SDL Window
	const int WINDOW_W = 800;
	const int WINDOW_H = 600;
	SDL_Window *window;
	window = SDL_CreateWindow("SDLWrap Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
	if(!window) {
		SDL_Log("Error creating SDL Window: %s", SDL_GetError());
		return false;
	}

	// Create SDL 2D renderer context
	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer) {
		SDL_Log("Error creating SDL Renderer: %s", SDL_GetError());
		return false;
	}

	// Create input state (scancode to use as index)
	std::array<bool, SDL_NUM_SCANCODES> keyDown{};

	// Application state
	bool quit = false;

	// Timing
	float lastFrameTimeMs = 0;

	// Object
	int x = 300;
	int y = 450;
	int w = 100;
	int h = 100;
	SDL_Rect rect = {x, y, w, h};

	const Uint8 r = 255;
	const Uint8 g = 0;
	const Uint8 b = 0;
	const Uint8 a = 255;
	SDL_Color rectColor = {r, g, b, a};

	float rectVx = 0;
	float rectVy = 0;

	const int PIXEL_PER_SECOND = 200;

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
		if(keyDown[SDL_SCANCODE_R]) {
			std::cout << "'R' key was pressed.\n";
			rectColor = {255, 0, 0, 255};
		}
		if(keyDown[SDL_SCANCODE_G]) {
			std::cout << "'G' key was pressed.\n";
			rectColor = {0, 255, 0, 255};
		}
		if(keyDown[SDL_SCANCODE_B]) {
			std::cout << "'B' key was pressed.\n";
			rectColor = {0, 0, 255, 255};
		}
		if(keyDown[SDL_SCANCODE_UP]) {
			std::cout << "Up arrow key is held down.\n";
			std::cout << "Rect XY: (" << rect.x << ", " << rect.y << ")\n";
			rect.y -= PIXEL_PER_SECOND * deltaTimeMs;
		}
		if(keyDown[SDL_SCANCODE_DOWN]) {
			std::cout << "Down arrow key is held down.\n";
			std::cout << "Rect XY: (" << rect.x << ", " << rect.y << ")\n";
			rect.y += PIXEL_PER_SECOND * deltaTimeMs;
		}
		if(keyDown[SDL_SCANCODE_LEFT]) {
			std::cout << "Left arrow key is held down.\n";
			std::cout << "Rect XY: (" << rect.x << ", " << rect.y << ")\n";
			rect.x -= PIXEL_PER_SECOND * deltaTimeMs;
		}
		if(keyDown[SDL_SCANCODE_RIGHT]) {
			std::cout << "Right arrow key is held down.\n";
			std::cout << "Rect XY: (" << rect.x << ", " << rect.y << ")\n";
			rect.x += PIXEL_PER_SECOND * deltaTimeMs;
		}

		// Update rect position
		rect.x += rectVx * deltaTimeMs;
		rect.y += rectVy * deltaTimeMs;

		// Decrease velocity by friction
		const float FRICTION = 0.9f;
		rectVx *= FRICTION;
		rectVy *= FRICTION;

		// Contrain ball to window bounds
		const int minX = 0;
		const int minY = 0;
		const int maxX = WINDOW_W - rect.w;
		const int maxY = WINDOW_H - rect.h;
		if(rect.x < minX) {
			rect.x = minX;
			rectVx = -rectVx;
		}
		if(rect.x + rect.w > WINDOW_W) {
			rect.x = maxX;
			rectVx = -rectVx;
		}
		if(rect.y < minY) {
			rect.y = minY;
			rectVy = -rectVy;
		}
		if(rect.y + rect.h > WINDOW_H) {
			rect.y = maxY;
			rectVy = -rectVy;
		}

		// Clear
		SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
		SDL_RenderClear(renderer);

		// Render rect
		SDL_SetRenderDrawColor(renderer, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
		SDL_RenderFillRect(renderer, &rect);

		// Render red pixel
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderDrawPoint(renderer, 300, 300);
		SDL_RenderDrawPoint(renderer, 301, 300);
		SDL_RenderDrawPoint(renderer, 302, 300);

		// Draw line
		SDL_RenderDrawLine(renderer, 400, 400, 500, 450);

		// Stroke rectangle
		SDL_Rect outlineRect = {600, 100, 150, 100};
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &outlineRect);

		// Circle
		// Not implemented in SDL2 core

		// Render text
		renderTextAt("Hello, SDL2!", font, renderer, 10, 10);
		renderTextAt("dt = " + std::to_string(deltaTimeMs), font, renderer, 10, 30);
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		renderTextAt("Mouse: (" + std::to_string(x) + ", " + std::to_string(y) + ")", font, renderer, 10, 50);
		int gx, gy;
		SDL_GetGlobalMouseState(&gx, &gy);
		renderTextAt("Global Mouse: (" + std::to_string(gx) + ", " + std::to_string(gy) + ")", font, renderer, 10, 70);

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
	return 0;
}
