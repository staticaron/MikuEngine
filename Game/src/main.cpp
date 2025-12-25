#include "Application.h"
#include "Layers/MainLayer.h"
#include "Logger.h"
#include "MikuEngine.h"

namespace MikuEngine
{
	Application* EntryPoint()
	{
		MIKU_INFO( "Welcome to RhythmGame" );

		Application* app = new Application();
		app->Init();

		app->PushLayer<RhythmGame::MainLayer>();

		app->Run();

		return app;
	}
}
