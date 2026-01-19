#pragma once

#include "Layers/EditorLayer.h"
#include "MikuEngine/AppLevelStuff.h"

namespace MikuEditor
{
	class EditorOverlayPanel
	{
	public:
		static void RenderEditorOverlayPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, EditorLevelStuff& editorLevelStuff, MikuEngine::Scene& scene );

	private:
	};
}
