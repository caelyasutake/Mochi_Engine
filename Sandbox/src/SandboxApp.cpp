#include <Mochi.h>

#include "imgui/imgui.h""

class ExampleLayer : public Mochi::Layer {
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {

		m_VertexArray.reset(Mochi::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		std::shared_ptr<Mochi::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mochi::VertexBuffer::Create(vertices, sizeof(vertices)));
		Mochi::BufferLayout layout = {
			{Mochi::ShaderDataType::Float3, "a_Position"},
			{Mochi::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Mochi::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mochi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Mochi::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f,
		};

		std::shared_ptr<Mochi::VertexBuffer> squareVB;
		squareVB.reset(Mochi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{Mochi::ShaderDataType::Float3, "a_Position"}
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2 , 2, 3, 0 };
		std::shared_ptr<Mochi::IndexBuffer> squareIB;
		squareIB.reset(Mochi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec4 v_Color;
			out vec3 v_Position;

			void main() {
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Mochi::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}	
		
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main() {
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}	
		
		)";

		m_BlueShader.reset(new Mochi::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate(Mochi::Timestep ts) override {

		// Movement
		if (Mochi::Input::IsKeyPressed(MC_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		else if (Mochi::Input::IsKeyPressed(MC_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		else if (Mochi::Input::IsKeyPressed(MC_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		// Rotation
		if (Mochi::Input::IsKeyPressed(MC_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Mochi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mochi::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Mochi::Renderer::BeginScene(m_Camera);

		Mochi::Renderer::Submit(m_BlueShader, m_SquareVA);
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

	float m_CameraRotation = 0.0f;
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