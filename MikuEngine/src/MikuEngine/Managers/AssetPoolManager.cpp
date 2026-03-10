#include "Managers/AssetPoolManager.h"

namespace MikuEngine
{
	void AssetPoolManager::Init()
	{
		m_TextureManager.LoadAllTextures();
		m_ShaderManager.LoadAllShaders();
		m_MaterialManager.LoadAllMaterials();
	}

	AssetType AssetPoolManager::GetAssetTypeFromPool( const UUID& uuid ) const
	{
		if ( m_TextureManager.TextureExists( uuid ) ) return AssetType::TEXTURE;
		if ( m_ShaderManager.ShaderExists( uuid ) ) return AssetType::SHADER;
		if ( m_MaterialManager.MaterialExists( uuid ) ) return AssetType::MATERIAL;

		return AssetType::NONE;
	}

	AssetType AssetPoolManager::GetAssetTypeFromFileExtension( const std::string& extension ) const
	{
		if ( extension == ".png" || extension == ".jpg" )
			return AssetType::TEXTURE;
		else if ( extension == ".scene" || extension == ".miku" )
			return AssetType::SCENE;
		else if ( extension == ".mat" )
			return AssetType::MATERIAL;
		else if ( extension == ".obj" )
			return AssetType::MODEL;
		else if ( extension == ".shader" )
			return AssetType::SHADER;
		else if ( extension == ".h" || extension == ".cpp" )
			return AssetType::SCRIPT;

		return AssetType::NONE;
	}
}
