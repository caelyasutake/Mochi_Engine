#pragma once

#include "Core.h"

#include "Window.h"
#include "Mochi/Core/LayerStack.h"
#include "Mochi/Events/Event.h"
#include "Mochi/Events/ApplicationEvent.h"

#include "Mochi/ImGui/ImGuiLayer.h"

#include "Mochi/Renderer/Shader.h"
#include "Mochi/Renderer/Buffer.h"
#include "Mochi/Renderer/VertexArray.h"

#include "Mochi/Core/Timestep.h"

#include "Mochi/Renderer/OrthographicCamera.h"

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
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		//std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}