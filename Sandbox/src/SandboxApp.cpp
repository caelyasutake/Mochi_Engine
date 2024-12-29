#include <Mochi.h>

class ExampleLayer : public Mochi::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}

	void OnUpdate() override {
		MC_INFO("ExampleLayer::Update");
	}

	void OnEvent(Mochi::Event& event) override {
		MC_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public Mochi::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Mochi::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Mochi::Application* Mochi::CreateApplication() {
	return new Sandbox();
}