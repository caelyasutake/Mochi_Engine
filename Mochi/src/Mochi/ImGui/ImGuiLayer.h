#pragma once

#include "Mochi/Layer.h"
#include "Mochi/Events/ApplicationEvent.h"
#include "Mochi/Events/KeyEvent.h"
#include "Mochi/Events/MouseEvent.h"

namespace Mochi {
	class MOCHI_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}