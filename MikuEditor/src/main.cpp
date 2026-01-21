#include "Application.h"
#include "Layers/EditorLayer.h"
#include "Layers/RendererLayer.h"
#include "Logger.h"
#include "MikuEngine.h"

namespace MikuEngine
{
	Application* EntryPoint()
	{
		MIKU_CLIENT_INFO( "Welcome to MikuEditor" );

		Application* app = new Application();
		app->Init();

		app->PushLayer<MikuEditor::RendererLayer>();
		app->PushLayer<MikuEditor::EditorLayer>();

		app->Run();

		return app;
	}
}
