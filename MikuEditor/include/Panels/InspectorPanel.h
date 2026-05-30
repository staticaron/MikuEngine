#pragma once

#include <filesystem>

#include "MikuEngine/Core.h"

namespace MikuEngine
{
	class Scene;
	class AppLevelStuff;
	class UUID;
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
		static void RenderFolderImGui( const std::filesystem::path& folderPath );
		static void RenderEntityInInspector( const MikuEngine::UUID& item, EditorLayer& editorLayer, MikuEngine::Scene& scene );
		static void RenderAssetInInspector( const MikuEngine::UUID& item, EditorLayer& editorLayer, MikuEngine::Scene& scene );
	};
}
