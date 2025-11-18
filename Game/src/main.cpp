#include "Application.h"
#include "MikuEngine.h"
#include "Scenes/MainScene.h"
#include "spdlog/spdlog.h"

namespace MikuEngine
{
	Application* EntryPoint()
	{
		spdlog::info( "Welcome to RhythmGame" );

		Application* app = new Application();
		app->Init();

		RhythmGame::MainScene mainScene;
		app->GetLayerStack().Push( mainScene );

		app->Run();

		return app;
	}
}
