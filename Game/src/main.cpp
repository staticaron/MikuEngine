#include "Application.h"
#include "MikuEngine.h"

MikuEngine::Application* MikuEngine::EntryPoint()
{
	Application* app = new Application();

	std::cout << "Welcome to Game" << std::endl;

	return app;
}
