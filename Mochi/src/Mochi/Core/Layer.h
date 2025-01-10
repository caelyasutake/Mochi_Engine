#pragma once

#include "Mochi/Core/Core.h"
#include "Mochi/Core/Timestep.h"
#include "Mochi/Events/Event.h"

namespace Mochi {
	class MOCHI_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}