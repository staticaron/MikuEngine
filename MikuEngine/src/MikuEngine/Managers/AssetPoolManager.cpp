#include "Managers/AssetPoolManager.h"

namespace MikuEngine
{
	void AssetPoolManager::Init()
	{
		m_TextureManager.LoadAllTextures();
		m_ShaderManager.LoadAllShaders();
		m_MaterialManager.LoadAllMaterials();
	}

	AssetType AssetPoolManager::GetAssetTypeFromPool( const UUID& uuid )
	{
		if ( m_TextureManager.TextureExists( uuid ) ) return AssetType::TEXTURE;
		if ( m_ShaderManager.ShaderExists( uuid ) ) return AssetType::SHADER;
		if ( m_MaterialManager.MaterialExists( uuid ) ) return AssetType::MATERIAL;

		return AssetType::NONE;
	}
}
