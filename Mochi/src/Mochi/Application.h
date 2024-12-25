#pragma once

#include "Core.h"

namespace Mochi {
	class MOCHI_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}