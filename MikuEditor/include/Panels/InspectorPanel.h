#pragma once

#include "MikuEngine/Core.h"
#include "MikuEngine/Scene/Scene.h"

#include "Layers/EditorLayer.h"

namespace MikuEditor
{
	class MIKU_API InspectorPanel
	{
	public:
		static void RenderInspectorPanel( EditorLayer& editorLayer, MikuEngine::Scene& scene );

	private:
	};
}
