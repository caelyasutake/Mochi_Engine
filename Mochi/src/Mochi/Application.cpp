#include "mcpch.h"
#include "Application.h"

#include "Mochi/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Mochi {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case Mochi::ShaderDataType::Float:
			return GL_FLOAT;
		case Mochi::ShaderDataType::Float2:
			return GL_FLOAT;
		case Mochi::ShaderDataType::Float3:
			return GL_FLOAT;
		case Mochi::ShaderDataType::Float4:
			return GL_FLOAT;
		case Mochi::ShaderDataType::Mat3:
			return GL_FLOAT;
		case Mochi::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Mochi::ShaderDataType::Int:
			return GL_INT;
		case Mochi::ShaderDataType::Int2:
			return GL_INT;
		case Mochi::ShaderDataType::Int3:
			return GL_INT;
		case Mochi::ShaderDataType::Int4:
			return GL_INT;
		case Mochi::ShaderDataType::Bool:
			return GL_BOOL;
		}

		MC_CORE_ASSERT(false, "Uknown ShaderDataType!");
		return 0;
	}

	Application::Application() {
		MC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		//m_ImGuiLayer = new ImGuiLayer();
		//PushOverlay(m_ImGuiLayer);
		m_ImGuiLayer = std::make_unique<ImGuiLayer>();

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();

		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(),
				(const void*)element.Offset);

			index++;
		}
	
		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;
			out vec3 v_Position;

			void main() {
				gl_Position = vec4(a_Position, 1.0);
				v_Color = a_Color;
				v_Position = a_Position;
			}	
		
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			in vec4 v_Color;
			in vec3 v_Position;

			out vec4 color;

			void main() {
				color = vec4(v_Position + 0.5, 1.0);
				color = v_Color;
			}	
		
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));


		// Vertex Array
		// Vertex Buffer
		// Index Buffer
		// ---
		// Shader
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		MC_CORE_TRACE("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false; 
		return true;
	}
}