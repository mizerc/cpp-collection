#include <stdlib.h>
#include <iostream>
#include <array>
#include <SDL.h>

#include "render.h"
#include "geometry.h"
#include "perlin3d.h"



// Video memory specs
const unsigned int VIDEO_HEIGHT = 128;
const unsigned int VIDEO_WIDTH = 128;

// Sphere specs
const int cx = VIDEO_WIDTH / 2;
const int cy = VIDEO_HEIGHT / 2;
const int radius = 32;
const int radiusSquared = radius * radius;

void setPixel(uint32_t *pVideoMemory, uint32_t x, uint32_t y, uint32_t color)
{
	uint32_t *pixelAddr = &pVideoMemory[y * VIDEO_WIDTH + x];
	*pixelAddr = color;
}

void paintTiledBackground(uint32_t *pVideoMemory)
{
	for(unsigned int y = 0; y < VIDEO_HEIGHT; ++y) {
		for(unsigned int x = 0; x < VIDEO_WIDTH; ++x) {
			setPixel(pVideoMemory, x, y, ((x ^ y) & 0x1) ? COLOR_LIGHT_GRAY : COLOR_RED);
		}
	}
}

void paintCloudCoverage(uint32_t *pVideoMemory, uint32_t x, uint32_t y)
{
	float nx, ny, nz;
	if(!pixelToSphereNormal((int)x, (int)y, cx, cy, radius, &nx, &ny, &nz))
		return;

	// Rotate the sample point
	// float angle = timeSeconds * 0.3f; // radians; tweak speed
	static float angle = 0.f;
	angle += 0.000001f; // rotation speed per frame
	rotateY(&nx, &ny, &nz, angle);

	// Clould around sphere
	// fbm3D is ~[-1,1] => map to [0,1]
	// Perlin to get terrain pattern
	const uint32_t cloudSeed = 1331;
	const float cloudFrequency = 1.0f; // bigger => smaller features
	const float cloudThreshold = 0.01;
	float cloudNoise = fbm3D(nx * cloudFrequency, ny * cloudFrequency, nz * cloudFrequency, cloudSeed);
	// From -1,+1 to 0,+1
	float v = 0.5f + 0.5f * cloudNoise;
	// Decide cloud coverage (soft threshold)
	float t = (v - cloudThreshold) / 0.15f;
	float coverage = smooth(clamp01(t));
	if(coverage <= 0.01f)
		return;

	// Alpha blend white over existing pixel
	uint32_t src = pVideoMemory[y * VIDEO_WIDTH + x];
	uint32_t dst = packRGB(255, 255, 255);
	// keep clouds subtle: scale coverage
	float alpha = clamp01(coverage * 0.75f);
	pVideoMemory[y * VIDEO_WIDTH + x] = lerpColor(src, dst, alpha);
}

void paintFrame(uint32_t *pVideoMemory)
{
	// Backgroiund
	// paintTiledBackground(pVideoMemory);

	// For each x,y
	for(unsigned int y = 0; y < VIDEO_HEIGHT; ++y) {
		for(unsigned int x = 0; x < VIDEO_WIDTH; ++x) {
			// Check if x,y is part of sphere
			// setPixel(pVideoMemory, 0, 0, COLOR_YELLOW);

			// Check if (x, y) is inside the sphere (circle)
			int dx = (int)x - cx;
			int dy = (int)y - cy;
			int dist = dx * dx + dy * dy;
			if(dist <= radiusSquared + 256) {
				setPixel(pVideoMemory, x, y, COLOR_DARK_YELLOW);
			}
			if(dist <= radiusSquared + 196) {
				setPixel(pVideoMemory, x, y, COLOR_YELLOW);
			}

			float nx, ny, nz;
			if(!pixelToSphereNormal((int)x, (int)y, cx, cy, radius, &nx, &ny, &nz))
				continue;

			// Rotate the sample point
			// float angle = timeSeconds * 0.3f; // radians; tweak speed
			static float angle = 0.f;
			angle += 0.000001f; // rotation speed per frame
			rotateY(&nx, &ny, &nz, angle);

			// Perlin to get terrain pattern
			const uint32_t terrainSeed = 1337;
			const float terrainFrequency = 3.0f; // bigger => smaller features
			float n = fbm3D(nx * terrainFrequency,
				ny * terrainFrequency,
				nz * terrainFrequency,
				terrainSeed);

			// Map to [0,1]
			float h = clamp01(0.5f + 0.5f * n);
			const uint32_t COLOR_OCEAN = 0x00102060;
			const uint32_t COLOR_LAND = 0x00207020;
			if(h < 0.48f)
				setPixel(pVideoMemory, x, y, COLOR_OCEAN); // ocean-ish
			else
				setPixel(pVideoMemory, x, y, COLOR_LAND); // land-ish

			paintCloudCoverage(pVideoMemory, x, y);
		}
	}
}

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
		SDL_Event e{};
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			// Update keyboard state
			if(e.type == SDL_KEYDOWN) {
				keyDown[e.key.keysym.scancode] = true;
			}
			if(e.type == SDL_KEYUP) {
				keyDown[e.key.keysym.scancode] = false;
			}
		}
		// React from keyboard state
		if(keyDown[SDL_SCANCODE_ESCAPE]) {
			quit = true;
		}

		paintFrame(videoMemory);

		// Copy bytes from videoMemory to sdlTexture
		SDL_UpdateTexture(sdlTexture, nullptr, videoMemory, sizeof(uint32_t) * VIDEO_WIDTH);
		// Clear renderer
		SDL_RenderClear(sdlRenderer);
		// Copy and scale texture to renderer; by default will scale the texture to fit the destination window size (stored in sdlRenderer)
		SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
		// Present renderer
		SDL_RenderPresent(sdlRenderer);
	}

	// Exit SDL cleanup
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();

	return EXIT_SUCCESS;
}
