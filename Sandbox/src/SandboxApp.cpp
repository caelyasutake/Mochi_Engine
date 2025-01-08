#include <Mochi.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h""
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Mochi::Layer {
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f), m_CameraRotation(0.0f) {

		// Add light properties
		m_LightColor = glm::vec3(1.5f, 1.5f, 1.5f);
		m_LightPosition = glm::vec3(1.0f, 1.0f, 2.0f);

		// Cube vertices (positions only)
		float vertices[10 * 8] = {
			// positions          
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 1
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 2
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 3
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 4
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 5
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 6
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // 7
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

		Mochi::Ref<Mochi::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mochi::VertexBuffer::Create(vertices, sizeof(vertices)));

		Mochi::BufferLayout layout = {
			{ Mochi::ShaderDataType::Float3, "a_Position" },
			{ Mochi::ShaderDataType::Float3, "a_Normal" },
			{ Mochi::ShaderDataType::Float2, "a_TexCoord" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Mochi::Ref<Mochi::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mochi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec3 a_Normal;
	
			uniform mat4 u_Transform;
			uniform mat4 u_ViewProjection;

			out vec3 v_Normal;
			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			in vec3 v_Normal;
			in vec3 v_Position;
			out vec4 color;

			uniform vec3 u_LightColor;
			uniform vec3 u_LightPos;

			uniform vec3 u_Color;

			void main() {
				float ambientStrength = 0.2;
				vec3 ambient = ambientStrength * u_LightColor;

				vec3 norm = normalize(v_Normal);
				vec3 lightDir = normalize(u_LightPos - v_Position);
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = diff * u_LightColor * 1.5;

				vec3 result = (ambient + diffuse) * u_Color;
				color = vec4(result, 1.0);
			}
		)";

		m_Shader.reset(Mochi::Shader::Create(vertexSrc, fragmentSrc));
		
		////////

		std::string textureVertexSrc = R"(
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec3 a_Normal;
			layout (location = 2) in vec2 a_TexCoord;

			uniform mat4 u_Transform;
			uniform mat4 u_ViewProjection;

			uniform sampler2D u_Texture;

			out vec3 v_Normal;
			out vec3 v_Position;
			out vec2 v_TexCoord;

			void main() {
				v_Position = a_Position;
				v_TexCoord = a_TexCoord;
				v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string textureFragmentSrc = R"(
			#version 330 core
			in vec3 v_Normal;
			in vec3 v_Position;
			in vec2 v_TexCoord;

			out vec4 color;

			uniform vec3 u_LightColor;
			uniform vec3 u_LightPos;
			uniform sampler2D u_Texture;

			void main() {
				float ambientStrength = 0.2;
				vec3 ambient = ambientStrength * u_LightColor;

				vec3 norm = normalize(v_Normal);
				vec3 lightDir = normalize(u_LightPos - v_Position);
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = diff * u_LightColor * 1.5;

				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Mochi::Shader::Create(textureVertexSrc, textureFragmentSrc));

		m_Texture = Mochi::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Mochi::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Mochi::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
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
			m_CameraRotation.y += m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_D))
			m_CameraRotation.y -= m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_W))
			m_CameraRotation.x += m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_S))
			m_CameraRotation.x -= m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_X))
			m_CameraRotation.z += m_CameraRotationSpeed * ts;

		if (Mochi::Input::IsKeyPressed(MC_KEY_C))
			m_CameraRotation.z -= m_CameraRotationSpeed * ts;

		Mochi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mochi::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Mochi::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Mochi::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Mochi::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_Color", m_CubeColor);
		std::dynamic_pointer_cast<Mochi::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_LightColor", m_LightColor);
		std::dynamic_pointer_cast<Mochi::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_LightPos", m_LightPosition);
		
		m_Texture->Bind();
		Mochi::Renderer::Submit(m_TextureShader, m_VertexArray);

		Mochi::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Cube Color", glm::value_ptr(m_CubeColor));
		ImGui::End();
	}

	void OnEvent(Mochi::Event& event) override {

	}

private:
	Mochi::Ref<Mochi::Shader> m_Shader, m_TextureShader;
	Mochi::Ref<Mochi::VertexArray> m_VertexArray;

	Mochi::Ref<Mochi::Texture2D> m_Texture;

	Mochi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;

	glm::vec3 m_CameraRotation;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_CubeColor = { 0.2f, 0.3f, 0.8f };

	glm::vec3 m_LightColor;
	glm::vec3 m_LightPosition;
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