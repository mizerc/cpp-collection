#include <iostream>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <linmath.h>

static void
error_callback(int error, const char *description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int
main()
{
	glfwSetErrorCallback(error_callback);

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

	// Store event callbacks
	glfwSetKeyCallback(window, key_callback);

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

	// ====== Set up shaders ======

	// Define vertex shader source
	const char *vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		uniform mat4 MVP;
		in vec3 aCol;
		out vec3 colorToNextShader;
		void main()
		{
			gl_Position = MVP * vec4(aPos, 1.0, 1.0);
			colorToNextShader = aCol;
		}
	)";

	// Define fragment shader source
	const char *fragmentShaderSource = R"(
		#version 330 core
		in vec3 colorToNextShader;
		out vec4 thepixelcolor;
		void main()
		{
			thepixelcolor = vec4(colorToNextShader, 1.0);
		}
	)";

	// Compile vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Check for compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "Vertex shader compilation failed:\n"
		          << infoLog << "\n";
	}

	// Compile fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "Fragment shader compilation failed:\n"
		          << infoLog << "\n";
	}

	// Link shaders into a program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader program linking failed:\n"
		          << infoLog << "\n";
	}

	// Delete shaders as they're linked now
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ====== Set up vertex data and buffers ======

	// Grab variables addresses
	const GLint mvp_location = glGetUniformLocation(shaderProgram, "MVP");
	const GLint vpos_location = glGetAttribLocation(shaderProgram, "aPos");
	const GLint vcol_location = glGetAttribLocation(shaderProgram, "aCol");

	// Vertex data for a triangle
	typedef struct Vertex
	{
		vec2 pos;
		vec3 col;
	} Vertex;
	static const Vertex vertices[3] =
	    {
	        // Bottom-left vertex - red
	        {{-0.5f, -0.5f}, {1.0f, 0.f, 0.f}},
	        // Bottom-right vertex - green
	        {{0.5f, -0.5f}, {0.f, 1.f, 0.f}},
	        // Top vertex - blue
	        {{0.0f, 0.5f}, {0.f, 0.f, 1.f}}};
	// Send vertex data to GPU
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Set up vertex array object
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));

	// Main loop
	while(!glfwWindowShouldClose(window))
	{
		// Get framebuffer size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		const float ratio = width / (float)height;
		glViewport(0, 0, width, height);

		// Set clear color and clear the buffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Send transformation matrices to the shader
		float time = (float)glfwGetTime();
		mat4x4 m, p, mvp;
		mat4x4_identity(m);
		mat4x4_rotate_Z(m, m, time);
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		// Draw call
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
