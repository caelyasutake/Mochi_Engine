#pragma once

#include "Core.h"

#include "Window.h"
#include "Mochi/LayerStack.h"
#include "Mochi/Events/Event.h"
#include "Mochi/Events/ApplicationEvent.h"

#include "Mochi/ImGui/ImGuiLayer.h"

#include "Mochi/Renderer/Shader.h"

namespace Mochi {
	class MOCHI_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		//ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;

		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}