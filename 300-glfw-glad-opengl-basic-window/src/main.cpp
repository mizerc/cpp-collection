#include <iostream>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void
processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int
main()
{
	// Initialize GLFW
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW\n";
		return -1;
	}

	// Set OpenGL version (3.3 Core)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow *window = glfwCreateWindow(800, 600, "Basic GLFW Window", nullptr, nullptr);
	if(!window)
	{
		std::cerr << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}

	// Make the OpenGL context current
	glfwMakeContextCurrent(window);
	// Enable vsync
	glfwSwapInterval(1);

	// Load OpenGL function pointers with GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}

	// Main loop
	while(!glfwWindowShouldClose(window))
	{
		// Set clear color and clear the buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
