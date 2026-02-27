#pragma once

#include <filesystem>

#include "glm/glm.hpp"

#include "MikuEngine/Core.h"
#include "MikuEngine/Data/SelectableItem.h"
#include "MikuEngine/Rendering/Texture.h"

namespace MikuEngine
{
	class Scene;
	enum class AssetType;
}

namespace MikuEditor
{
	class MIKU_API AssetBrowserPanel
	{
	public:
		void Init();
		void RenderAssetBrowserPanel( MikuEngine::Scene& scene );

		void RenderFolderIcon( const std::filesystem::path& folderPath );
		void RenderFileIcon( const std::filesystem::path& folderPath, std::function<void( MikuEngine::UUID, MikuEngine::AssetType )> onClickFunc );

	private:
		std::filesystem::path m_ContentBrowserLocation = PROJECT_DIR;
		std::filesystem::path m_RootAssetLocation = PROJECT_DIR;

		std::unordered_map<MikuEngine::AssetType, MikuEngine::Texture> m_IconTextures;

		int m_IconSize = 100;
	};
}
