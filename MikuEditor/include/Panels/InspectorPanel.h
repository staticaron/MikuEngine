#pragma once

#include "MikuEngine/Core.h"

namespace MikuEngine
{
	class Scene;
	class AppLevelStuff;
}

namespace MikuEditor
{
	class EditorLayer;
}

namespace MikuEditor
{
	class MIKU_API InspectorPanel
	{
	public:
		static void RenderInspectorPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene );

	private:
	};
}
