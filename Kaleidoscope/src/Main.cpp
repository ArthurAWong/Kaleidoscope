#ifdef KS_PLATFORM_WINDOWS
#include <windows.h>
#include <cmath>
#include <map>
#include <vector>
#include <filesystem>
#include "Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Eigen/Eigen"
#include "CSV.h"

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);
int glfwTestCode();
int outputToCSV(char* fileName);

const unsigned int drawTriangle = 1;

int main(int argc, char** argv) {

	Kaleidoscope::Log::Init();
	KS_CORE_INFO("Initialized log");

	// Declaring physical constants in MKS units, and free-electron mass m

	const double hbar = 1.054 * pow(10, -34);
	const double q = 1.602 * pow(10, -19);
	const double m = 9.11 * pow(10, -31);

	/*
	Lattice data: total number N of lattice points, a grid of integers from n=1 to n=N;
	lattice constant "a" (lattice spacing); a discretized position grid x; Hamiltonian
	parameter t0 (as derived using central difference) divided by q to have units of eV;
	and total length of the lattice, L= a*(N+1). Periodic boundary condition,
	point N = point 0, point N+1 = point 1
	*/

	const int N = 100;
	Eigen::ArrayXd n = Eigen::ArrayXd::LinSpaced(N, 1, N);
	const double a = 1 * pow(10, -10);
	Eigen::ArrayXd x = n * a;
	const double t0 = pow(hbar, 2) / (2 * m * pow(a, 2)) / q;

	// Set up the Hamiltonian matrix

	Eigen::MatrixXd H(N, N);
	// In reality, U would be an array of size N and replace 0 
	H.diagonal(0).setConstant(2 * t0 + 0);
	H.diagonal(1).setConstant(-t0);
	H.diagonal(-1).setConstant(-t0);

	Eigen::EigenSolver<Eigen::MatrixXd> EigenSol(H);

	//std::cout << "The eigenvalues of H is " << EigenSol.eigenvalues() << std::endl;
	//std::cout << "The eigenvectors of H is " << EigenSol.eigenvectors() << std::endl;
	Eigen::MatrixXcd eigenvectors = EigenSol.eigenvectors();
	Eigen::VectorXcd eigenvector1 = eigenvectors.col(1);
	std::vector<double> normSqrEigenvector1(eigenvector1.size());
	int i = 0;
	for (const Eigen::dcomplex& compNum : eigenvector1)
	{
		normSqrEigenvector1[i] = std::norm(compNum);
		i++;
	}

	std::vector<double> vecN(normSqrEigenvector1.size());
	std::iota(vecN.begin(), vecN.end(), 1);
	CSVFrame TestFrame({ {"N", vecN}, {"n=1", normSqrEigenvector1} });

	TestFrame.WriteToCSV("example.csv");

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

int glfwTestCode() {
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
}

//int outputToCSV(char* fileName, void** colNames,d void** iterables) {

//}

#endif