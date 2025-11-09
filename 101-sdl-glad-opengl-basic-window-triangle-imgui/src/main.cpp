#include <stdlib.h>
#include <iostream>
#include <array>
// SDL
#include <SDL.h>
// OPENGL
// #include <SDL_opengl.h> // Not used when using glad
#include "glad/glad.h"
// IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

const int WINDOW_W = 800;
const int WINDOW_H = 600;

// Minimal shaders
const char *vs_src = R"(
#version 410 core
layout (location = 0) in vec2 aPos;
void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";
const char *fs_src = R"(
#version 410 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0, 0.2, 0.3, 1.0);
}
)";

int main(int argc, char **argv)
{
	std::cout << "hi\n";

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	// Set OpenGL version (4.1 Core)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create SDL Window
	SDL_Window *window;
	window = SDL_CreateWindow(
		"SDL OpenGL Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W,
		WINDOW_H,
		SDL_WINDOW_OPENGL);
	// SDL_WINDOW_RESIZABLE
	if(!window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
		SDL_Log("Error creating SDL Window: %s", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// Create context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if(!glContext) {
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// Load OpenGL via GLAD
	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to load GL\n";
		return EXIT_FAILURE;
	}

	// Enable Vsync
	SDL_GL_SetSwapInterval(1);

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	// Pass GLSL version (410 core for macOS OGL 4.1)
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 410");
	// Set ImGui style
	ImGui::StyleColorsDark();

	// Create shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_src, nullptr);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_src, nullptr);
	glCompileShader(fs);

	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	glDeleteShader(vs);
	glDeleteShader(fs);

	// Triangle data
	float verts[] = {
		0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f};

	// Create VAO and VBO
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Create input state (scancode to use as index)
	std::array<bool, SDL_NUM_SCANCODES> keyDown{};

	// Application state
	bool quit = false;

	// Timing
	float lastFrameTimeMs = 0;

	// Set viewport
	glViewport(0, 0, WINDOW_W, WINDOW_H);

	// Main loop
	while(!quit) {
		// Update delta time
		Uint32 nowMs = SDL_GetTicks();
		float deltaTimeMs = (nowMs - lastFrameTimeMs) / 1000.0f;
		lastFrameTimeMs = nowMs;

		// Poll events from queue
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			// Pass event to ImGui
			ImGui_ImplSDL2_ProcessEvent(&e);
			// Handle SDL events
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

		// Clear
		glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render a triangle using old opengl (not supported in MacOS)
		// glBegin(GL_TRIANGLES);
		// glColor3f(1, 0, 0);
		// glVertex2f(0, 0.5f);
		// glVertex2f(-0.5f, -0.5f);
		// glVertex2f(0.5f, -0.5f);
		// glEnd();

		// Render a triangle using modern OpenGL (with glad)
		glUseProgram(prog);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ImGui new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		// UI
		ImGui::Begin("Test");
		ImGui::Text("Hello from ImGui + OpenGL 4.1!");
		ImGui::End();

		// Another window
		ImGui::Begin("Another Window"); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		ImGui::Button("Close Me");
		ImGui::End();

		// Demo windows
		ImGui::ShowDemoWindow();

		// Render
		ImGui::Render();

		// backend render call
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		SDL_GL_SwapWindow(window);

		// Delay to cap frame rate
		Uint32 delayDurationMs = 16; // Approx ~60 FPS
		SDL_Delay(delayDurationMs);
	}

	// Imgui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// Cleanup OpenGL resources
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(prog);

	// Cleanup SDL
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
