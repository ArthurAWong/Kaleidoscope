#ifdef KS_PLATFORM_WINDOWS
#include <windows.h>
#include "Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);
/*
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"	ourColor = aColor;\n"
	"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main() {\n"
	"	FragColor = vec4(ourColor, 1.0);\n"
	"}\0";
*/
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

	/*

	// Vertex shader 
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Verify if vertex shader compile works
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		KS_CORE_CRITICAL("Failed to compile shader vertex");
		KS_CORE_CRITICAL("{}",infoLog);
	}
	// Fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Verify if fragment shader compile works
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		KS_CORE_CRITICAL("Failed to compile fragment shader");
		KS_CORE_CRITICAL("{}", infoLog);
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Verify if linking shaders works
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		KS_CORE_CRITICAL("Failed to link shaders");
		KS_CORE_CRITICAL("{}", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	*/

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

		//float redValue = sin(counter/100) / 2.0f + 0.5f;
		//counter++;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		//glUseProgram(shaderProgram);
		ourShader.use();
		//glUniform4f(vertexColorLocation, 0.5f, redValue, 0.0f, 1.0f);

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