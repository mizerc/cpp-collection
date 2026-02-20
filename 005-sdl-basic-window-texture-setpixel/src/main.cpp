#include <stdlib.h>
#include <iostream>
#include <array>
#include <SDL.h>

const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

int main(int argc, char **argv)
{
	if(argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <Delay> <Scale> <ROM>\n";
		return EXIT_FAILURE;
	}
	int frameDurationTargetMs = std::stoi(argv[1]);
	int videoScale = std::stoi(argv[2]);
	char const *romFilename = argv[3];

	uint32_t videoMemory[VIDEO_WIDTH * VIDEO_HEIGHT]{};
	SDL_Window *sdlWindow{};
	SDL_Renderer *sdlRenderer{};
	SDL_Texture *sdlTexture{};

	const int WINDOW_W = VIDEO_WIDTH * videoScale;
	const int WINDOW_H = VIDEO_HEIGHT * videoScale;

	std::cout << "hi\n";

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	// Create SDL Window
	sdlWindow = SDL_CreateWindow(
		"SDL OpenGL Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W,
		WINDOW_H,
		SDL_WINDOW_RESIZABLE);
	// SDL_WINDOW_RESIZABLE
	if(!sdlWindow) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
		SDL_Log("Error creating SDL Window: %s", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// Initialize SDL Renderer
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);

	// Initialize SDL Texture
	sdlTexture = SDL_CreateTexture(
		sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);

	// Create input state (scancode to use as index)
	std::array<bool, SDL_NUM_SCANCODES> keyDown{};

	// Application state
	bool quit = false;

	// Timing
	float lastFrameTimeMs = 0;

	// Main loop
	while(!quit) {
		// Time control
		Uint32 nowMs = SDL_GetTicks();
		float deltaTimeMs = (nowMs - lastFrameTimeMs);
		// Wait to meet cycle delay
		if(deltaTimeMs < frameDurationTargetMs) {
			continue;
		}
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
		for(unsigned int y = 0; y < VIDEO_HEIGHT; ++y) {
			for(unsigned int x = 0; x < VIDEO_WIDTH; ++x) {
				uint32_t *pixelAddr = &videoMemory[y * VIDEO_WIDTH + x];
				// *pixelAddr = 0xFFFF00FF;
				*pixelAddr = ((x ^ y) & 0x1) ? 0xFFFFFFFF : 0xFF000000;
			}
		}

		// Copy video memory to texture
		int rowSizeBytes = sizeof(uint32_t) * VIDEO_WIDTH;
		SDL_UpdateTexture(sdlTexture, nullptr, videoMemory, rowSizeBytes);
		// Clear renderer
		SDL_RenderClear(sdlRenderer);
		// Copy texture to renderer
		// by default will scalethe texture to fit the destination window size
		SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
		// Present renderer
		SDL_RenderPresent(sdlRenderer);
	}

	// Cleanup SDL
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();

	return EXIT_SUCCESS;
}
