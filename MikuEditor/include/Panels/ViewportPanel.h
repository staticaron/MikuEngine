#pragma once

#include "imgui.h"
#include "ImGuizmo.h"
#include "MikuEngine/Core.h"

namespace MikuEngine
{
	class Scene;
}

namespace MikuEditor
{
	class EditorLayer;
}

namespace MikuEditor
{
	class MIKU_API ViewportPanel
	{
	public:
		void Update();

		bool RenderViewportPanel( EditorLayer& editorLayer, MikuEngine::Scene& scene );

	private:
		ImGuizmo::OPERATION m_CurrentOperation = ImGuizmo::OPERATION::TRANSLATE;
	};
}
