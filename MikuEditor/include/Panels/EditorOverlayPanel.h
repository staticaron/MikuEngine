#pragma once

namespace MikuEngine
{
	class AppLevelStuff;
	class Scene;
}

namespace MikuEditor
{
	class EditorLayer;
	class EditorLevelStuff;
}

namespace MikuEditor
{
	class EditorOverlayPanel
	{
	public:
		static void RenderEditorOverlayPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, EditorLevelStuff& editorLevelStuff, MikuEngine::Scene& scene );

	private:
	};
}
