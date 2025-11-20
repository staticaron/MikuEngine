#include "Application.h"
#include "Layers/MainLayer.h"
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

		app->PushLayer<RhythmGame::MainLayer>();
		app->GetLayer( 0 )->PushScene<RhythmGame::MainScene>();

		app->Run();

		return app;
	}
}
