#ifdef KS_PLATFORM_WINDOWS
#include <windows.h>
#include "Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int drawTriangle = 1;

int main(int argc, char** argv) {

	Kaleidoscope::Log::Init();
	KS_CORE_INFO("Initialized log");

	if (!glfwInit()) {
		KS_CORE_CRITICAL("Failed to initialize GLFW");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		KS_CORE_CRITICAL("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		KS_CORE_CRITICAL("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	Shader ourShader("F:/Dev/Kaleidoscope/Kaleidoscope/src/Shaders/testVert.vert", "F:/Dev/Kaleidoscope/Kaleidoscope/src/Shaders/testFrag.frag");

	// Set up vertex data, buffers and configure vertex attributes
	// If drawTriangle is false, draws rectangle 
	unsigned int VBO, VAO, EBO;
	if (drawTriangle) {
		float verticies[] = {
		// Position			// Color
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// Bottom left
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// Bottom right
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f	// Top
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// First bind VAO, then bind and set vertex buffers, and then configure vertex attributes
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else {
		float verticies[] = {
		  0.5f,  0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
		};
		unsigned int indices[] = {
			0, 1, 3,	//first triangle
			1, 2, 3,	//second triangle
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	} 

	float counter = 0;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.6f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		glBindVertexArray(VAO);
		if (drawTriangle) {
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up resources that no longer have a use

	glDeleteVertexArrays(1, &VAO);
	if (drawTriangle) {
		glDeleteBuffers(1, &VBO);
	}
	else {
		glDeleteBuffers(1, &EBO);
	}
	ourShader.~Shader();

	glfwTerminate();
	KS_CORE_INFO("Closed window");
	return 0;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

#endif