#pragma once

#ifdef MC_PLATFORM_WINDOWS

extern Mochi::Application* Mochi::CreateApplication();

int main(int argc, char** argv) {
	Mochi::Log::Init();
	MC_CORE_WARN("Initialized Log!");
	int a = 5;
	MC_INFO("Hello Var={0}", a);

	auto app = Mochi::CreateApplication();
	app->Run();
	delete app;
}

#endif