#pragma once

#include <filesystem>

#include "MikuEngine/Core.h"
#include "MikuEngine/Rendering/Texture.h"

namespace MikuEngine
{
	class Scene;
	enum class AssetType;
}

namespace MikuEditor
{
	class EditorLayer;
}

namespace MikuEditor
{
	class MIKU_API AssetBrowserPanel
	{
	public:
		void Init();
		void RenderAssetBrowserPanel( EditorLayer& editorLayer, MikuEngine::Scene& scene );

		void RenderFolderIcon( const std::filesystem::path& folderPath, std::function<void()> onClickFunc );
		void RenderFileIcon( const std::filesystem::path& folderPath, std::function<void( MikuEngine::UUID, MikuEngine::AssetType, const std::filesystem::path& )> onClickFunc );

	private:
		std::filesystem::path m_ContentBrowserLocation = PROJECT_DIR;
		std::filesystem::path m_RootAssetLocation = PROJECT_DIR;

		std::unordered_map<MikuEngine::AssetType, MikuEngine::Texture> m_IconTextures;

		int m_IconSize = 64;
	};
}
