#pragma once

#ifdef KS_PLATFORM_WINDOWS

extern Kaleidoscope::Application* Kaleidoscope::CreateApplication();

int main(int argc, char** argv) {
	
	auto app = Kaleidoscope::CreateApplication();
	app->Run();
	delete app;
}

#endif