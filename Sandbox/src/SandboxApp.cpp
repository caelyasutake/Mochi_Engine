#include <Mochi.h>

class Sandbox : public Mochi::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Mochi::Application* Mochi::CreateApplication() {
	return new Sandbox();
}