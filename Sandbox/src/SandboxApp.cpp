#include <Mochi.h>

#include "imgui/imgui.h""

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Mochi::Layer {
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f), m_CameraRotation(0.0f) {

		// Cube vertices (positions only)
		float vertices[3 * 8] = {
			// positions          
			-0.5f, -0.5f, -0.5f,  // 0
			 0.5f, -0.5f, -0.5f,  // 1
			 0.5f,  0.5f, -0.5f,  // 2
			-0.5f,  0.5f, -0.5f,  // 3
			-0.5f, -0.5f,  0.5f,  // 4
			 0.5f, -0.5f,  0.5f,  // 5
			 0.5f,  0.5f,  0.5f,  // 6
			-0.5f,  0.5f,  0.5f   // 7
		};

		// Cube indices for 12 triangles (6 faces, 2 triangles per face)
		uint32_t indices[6 * 6] = {
			0, 1, 2, 2, 3, 0, // Front face
			1, 5, 6, 6, 2, 1, // Right face
			5, 4, 7, 7, 6, 5, // Back face
			4, 0, 3, 3, 7, 4, // Left face
			3, 2, 6, 6, 7, 3, // Top face
			4, 5, 1, 1, 0, 4  // Bottom face
		};

		m_VertexArray.reset(Mochi::VertexArray::Create());

		std::shared_ptr<Mochi::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mochi::VertexBuffer::Create(vertices, sizeof(vertices)));

		Mochi::BufferLayout layout = {
			{ Mochi::ShaderDataType::Float3, "a_Position" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<Mochi::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mochi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			layout (location = 0) in vec3 a_Position;
	
			uniform mat4 u_Transform;
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			out vec4 color;

			void main() {
				color = vec4(1.0, 0.5, 0.2, 1.0);
			}	
		)";

		m_Shader.reset(new Mochi::Shader(vertexSrc, fragmentSrc));

	}

	void OnUpdate(Mochi::Timestep ts) override {

		// Camera Movement
		if (Mochi::Input::IsKeyPressed(MC_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		else if (Mochi::Input::IsKeyPressed(MC_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		else if (Mochi::Input::IsKeyPressed(MC_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		// Camera Rotation
		if (Mochi::Input::IsKeyPressed(MC_KEY_A))
			m_CameraRotation.z += m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_D))
			m_CameraRotation.z -= m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_W))
			m_CameraRotation.y += m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_S))
			m_CameraRotation.y -= m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_X))
			m_CameraRotation.x += m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_C))
			m_CameraRotation.x -= m_CameraRotationSpeed * ts;

		Mochi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mochi::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Mochi::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * scale;
		
		Mochi::Renderer::Submit(m_Shader, m_VertexArray);

		Mochi::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {

	}

	void OnEvent(Mochi::Event& event) override {

	}

private:
	std::shared_ptr<Mochi::Shader> m_Shader;
	std::shared_ptr<Mochi::VertexArray> m_VertexArray;

	std::shared_ptr<Mochi::Shader> m_BlueShader;
	std::shared_ptr<Mochi::VertexArray> m_SquareVA;

	Mochi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;

	glm::vec3 m_CameraRotation;
	float m_CameraRotationSpeed = 180.0f;
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