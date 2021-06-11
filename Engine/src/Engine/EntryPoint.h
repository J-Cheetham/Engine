#pragma once

#ifdef ENG_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char**argv)
{
	Engine::Log::Init();
	ENG_CORE_WARN("Initialised Log!");
	int a = 5;
	ENG_CORE_INFO("Hello! Var={0}", a);

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif