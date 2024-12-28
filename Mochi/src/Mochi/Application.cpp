#include "Application.h"

#include "Mochi/Events/ApplicationEvent.h"
#include "Mochi/Log.h"

namespace Mochi {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		MC_TRACE(e.ToString());

		while (true);
	}
}