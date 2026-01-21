#pragma once

#include "Core.h"

namespace MikuEngine
{
	class AppLevelStuff;
	class Scene;
}

namespace MikuEditor
{
	class EditorLevelStuff;
}

namespace MikuEditor
{
	class EditorLayer;

	class MIKU_API MenuBar
	{
	public:
		static void RenderMenuBar( EditorLayer& editorlayer, const MikuEngine::AppLevelStuff& appLevelStuff, EditorLevelStuff& editorLevelStuff, MikuEngine::Scene& scene );

	private:
	};
}
