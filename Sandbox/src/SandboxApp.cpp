#include <Kaleidoscope.h>

class Sandbox : public Kaleidoscope::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Kaleidoscope::Application* Kaleidoscope::CreateApplication() {

	return new Sandbox();

}