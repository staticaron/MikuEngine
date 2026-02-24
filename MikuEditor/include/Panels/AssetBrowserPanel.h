#pragma once

#include <filesystem>

#include "glm/glm.hpp"

#include "MikuEngine/Core.h"
#include "MikuEngine/Rendering/Texture.h"

namespace MikuEngine
{
	class Scene;
}

namespace MikuEditor
{
	class MIKU_API AssetBrowserPanel
	{
	public:
		void Init();
		void RenderAssetBrowserPanel( MikuEngine::Scene& scene );

	private:
		std::filesystem::path m_ContentBrowserLocation = PROJECT_DIR;
		std::filesystem::path m_RootAssetLocation = PROJECT_DIR;

		std::unordered_map<MikuEngine::AssetType, MikuEngine::Texture> m_IconTextures;

		glm::vec2 m_IconSize = { 100, 100 };
	};
}
