#ifdef KS_PLATFORM_WINDOWS

#include "Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <windows.h>

int main(int argc, char** argv) {

	Kaleidoscope::Log::Init();
	KS_CORE_WARN("Initialized Log!");
	int a = 10;
	KS_TRACE("Kaleidoscope Engine = {0}", a);

	glfwInit();
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, GLF	);

	while (true) {}

	return 0;
}

#endif