#pragma once

#include <filesystem>

#include "MikuEngine/Core.h"

namespace MikuEngine
{
	class Scene;
}

namespace MikuEditor
{
	class MIKU_API AssetBrowserPanel
	{
	public:
		static void RenderAssetBrowserPanel( MikuEngine::Scene& scene );

	private:
		inline static std::filesystem::path m_ContentBrowserLocation = PROJECT_DIR;
		inline static std::filesystem::path m_RootAssetLocation = PROJECT_DIR;
	};
}
