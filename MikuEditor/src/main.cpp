#include "Application.h"
#include "Layers/EditorLayer.h"
#include "Logger.h"
#include "MikuEngine.h"

namespace MikuEngine
{
	Application* EntryPoint()
	{
		MIKU_INFO( "Welcome to MikuEditor" );

		Application* app = new Application();
		app->Init();

		app->PushLayer<MikuEditor::EditorLayer>();

		app->Run();

		return app;
	}
}
