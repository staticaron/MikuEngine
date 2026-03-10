#pragma once

#include "Core.h"
#include "Managers/MaterialManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

namespace MikuEngine
{
	class MIKU_API AssetPoolManager
	{
	public:
		void Init();

		AssetType GetAssetTypeFromPool( const UUID& uuid ) const;

		TextureManager& GetTextureManager() { return m_TextureManager; }
		const TextureManager& GetTextureManager() const { return m_TextureManager; }

		ShaderManager& GetShaderManager() { return m_ShaderManager; }
		const ShaderManager& GetShaderManager() const { return m_ShaderManager; }

		MaterialManager& GetMaterialManager() { return m_MaterialManager; }
		const MaterialManager& GetMaterialManager() const { return m_MaterialManager; }

		AssetType GetAssetTypeFromFileExtension( const std::string& extension ) const;

	private:
		TextureManager m_TextureManager;
		ShaderManager m_ShaderManager;
		MaterialManager m_MaterialManager;
	};
}
