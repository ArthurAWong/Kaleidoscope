#ifdef KS_PLATFORM_WINDOWS
#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <vector>
#include <filesystem>
#include <numeric>
#include "Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Eigen/Eigen"
#include "CSV.h"
#include "EigenUtil.h"

void FrameBufferSizeCallback(GLFWwindow*, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
int GlfwTestCode();
int Eigen3TestCode();

Eigen::Vector3f cameraPos(0.0f, 0.0f, 3.0f);
Eigen::Vector3f cameraFront(0.0f, 0.0f, -1.0f);
Eigen::Vector3f cameraUp(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400;
float lastY = 300;

float yaw = -90.0f;
float pitch = 0.0f;

float fov = 45.0f;

bool firstMouse = true;

int main(int argc, char** argv) {

	Kaleidoscope::Log::Init();
	KS_CORE_INFO("Initialized log");

	GlfwTestCode();

	return 0;
}

void FrameBufferSizeCallback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
	float cameraSpeed = 7.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront.cross(cameraUp).normalized();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront.cross(cameraUp).normalized();
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xOffset = (float)xpos - lastX;
	float yOffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	
	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	Eigen::Vector3f front(
		std::cos(Radians(yaw)) * std::cos(Radians(pitch)),
		std::sin(Radians(pitch)),
		std::sin(Radians(yaw)) * std::cos(Radians(pitch))
	);

	cameraFront = front.normalized();

}

void ScrollCallback(GLFWwindow* window, double xcallback, double ycallback) {
	fov -= (float)ycallback;
	
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 179.0f) {
		fov = 179.0f;
	}
}

int GlfwTestCode() {

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
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		KS_CORE_CRITICAL("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("C:/Dev/Kaleidoscope/Kaleidoscope/src/Shaders/testVert.vert", "C:/Dev/Kaleidoscope/Kaleidoscope/src/Shaders/testFrag.frag");

	float verticies[] = {
		// Position			// Color
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,	
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,	
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f

	};
	unsigned int indices[] = {
		1, 3, 5, // bottom
		3, 5, 7,
		0, 2, 4, // top
		2, 4, 6,
		0, 1, 2, // pos x-axis
		1, 2, 3,
		4, 5, 6, // neg x-axis
		5, 6, 7,
		0, 1, 4, // pos y-axis
		1, 4, 5,
		2, 3, 6, // neg y-axis
		3, 6, 7

	};
	// Set up vertex data, buffers and configure vertex attributes
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// First bind VAO, then bind and set vertex buffers, and then configure vertex attributes
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Eigen::Transform<float, 3, Eigen::Affine> model;
	model.setIdentity();
	model = Eigen::AngleAxis<float>(Radians(-45.0f), Eigen::Vector3f(1, 0, 0));
	unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");

	Eigen::Transform<float, 3, Eigen::Affine> projection;
	unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");

	Eigen::Transform<float, 3, Eigen::Affine> view;
	unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		ProcessInput(window);

		glClearColor(0.6f, 0.5f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		projection = Perspective<float>(Radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		view = LookAt<Eigen::Vector3f>(cameraPos, cameraPos + cameraFront, cameraUp);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up resources that no longer have a use

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	ourShader.~Shader();

	glfwTerminate();
	KS_CORE_INFO("Closed window");

	return 0;
}

int Eigen3TestCode() {
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
	const double a = pow(10, -10);
	Eigen::ArrayXd x = n * a;
	const double t0 = pow(hbar, 2) / (2 * m * pow(a, 2)) / q;

	// Set up the Hamiltonian matrix

	Eigen::MatrixXd H(N, N);
	Eigen::VectorXd U(N);

	H.diagonal(0) +
		H.diagonal(0).setConstant(2 * t0);
	H.diagonal(0) + U;
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

#endif