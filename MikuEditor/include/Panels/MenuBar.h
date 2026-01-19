#pragma once

#include "Core.h"

#include "Layers/EditorLevelStuff.h"
#include "Scene/Scene.h"

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
