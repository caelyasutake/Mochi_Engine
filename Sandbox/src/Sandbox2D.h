#pragma once

#include "Mochi.h"

class Sandbox2D : public Mochi::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Mochi::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Mochi::Event& e) override;

private:
	Mochi::OrthographicCameraController m_CameraController;

	// Temp
	Mochi::Ref<Mochi::VertexArray> m_SquareVA;
	Mochi::Ref<Mochi::Shader> m_FlatColorShader;

	Mochi::Ref<Mochi::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.0f, 1.0f };
};