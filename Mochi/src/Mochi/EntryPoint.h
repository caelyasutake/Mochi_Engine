#pragma once

#ifdef MC_PLATFORM_WINDOWS

extern Mochi::Application* Mochi::CreateApplication();

int main(int argc, char** argv) {
	//printf("Mochi Engine\n");
	auto app = Mochi::CreateApplication();
	app->Run();
	delete app;
}

#endif