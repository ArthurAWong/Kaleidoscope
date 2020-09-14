#pragma once

#ifdef KS_PLATFORM_WINDOWS

extern Kaleidoscope::Application* Kaleidoscope::CreateApplication();

int main(int argc, char** argv) {
	
	Kaleidoscope::Log::Init();
	KS_CORE_WARN("Initialized Log!");
	int a = 10;
	KS_TRACE("Kaleidoscope Engine = {0}", a);

	auto app = Kaleidoscope::CreateApplication();
	app->Run();
	delete app;
}

#endif