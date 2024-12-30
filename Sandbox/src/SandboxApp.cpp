#include <Mochi.h>

#include "imgui/imgui.h""

class ExampleLayer : public Mochi::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}

	void OnUpdate() override {
		//MC_INFO("ExampleLayer::Update");

		if (Mochi::Input::IsKeyPressed(MC_KEY_TAB))
			MC_TRACE("Tab key is pressed!");
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}

	void OnEvent(Mochi::Event& event) override {
		//MC_TRACE("{0}", event.ToString());
		if (event.GetEventType() == Mochi::EventType::KeyPressed) {
			Mochi::KeyPressedEvent& e = (Mochi::KeyPressedEvent&)event;
			MC_TRACE("{0}", (char)e.GetKeyCode());
		}
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