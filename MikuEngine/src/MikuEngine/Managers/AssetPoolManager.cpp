#include "Managers/AssetPoolManager.h"

#include "Logger.h"

namespace MikuEngine
{
	void AssetPoolManager::Init()
	{
		m_TextureManager.Init();
		m_ShaderManager.Init();
		m_MaterialManager.Init();
		m_ModelManager.Init();
	}

	void AssetPoolManager::InitFrame()
	{
		GetTextureManager().InitFrame();
		GetShaderManager().InitFrame();
		GetMaterialManager().InitFrame();
		GetModelManager().InitFrame();
	}

	AssetType AssetPoolManager::GetAssetTypeFromPool( const UUID& uuid ) const
	{
		if ( m_TextureManager.TextureExists( uuid ) )
			return AssetType::TEXTURE;
		if ( m_ShaderManager.ShaderExists( uuid ) )
			return AssetType::SHADER;
		if ( m_MaterialManager.MaterialExists( uuid ) )
			return AssetType::MATERIAL;
		if ( m_ModelManager.ModelExists( uuid ) )
			return AssetType::MODEL;

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
		else if ( extension == ".fbx" )
			return AssetType::MODEL;

		return AssetType::NONE;
	}

	void AssetPoolManager::LoadAssetFromFile( const std::filesystem::path& filePath )
	{
		AssetType type = GetAssetTypeFromFileExtension( filePath.extension() );

		switch ( type )
		{
		case AssetType::TEXTURE:
			m_TextureManager.LoadTexture( filePath );
			break;
		case AssetType::MODEL:
			m_ModelManager.LoadModel( filePath );
			break;
		case AssetType::SHADER:
			m_ShaderManager.LoadShader( filePath );
			break;
		case AssetType::MATERIAL:
			m_MaterialManager.LoadMaterial( filePath );
			break;
		case AssetType::SCENE:
			MIKU_CORE_WARN( "Hot Reloading of Scene is not implemented yet!" );
			break;
		case AssetType::SCRIPT:
			MIKU_CORE_WARN( "Hot Reloading of Script is not implemented yet!" );
			break;
		case AssetType::NONE:
			MIKU_CORE_WARN( "Hot Reloading of this filetype is not implemented yet!" );
			break;
		default:
			MIKU_CORE_WARN( "Hot Reloading of this filetype is not implemented yet!" );
			break;
		}
	}

	void AssetPoolManager::RemoveAssetOfFile( const std::filesystem::path& filePath )
	{
		AssetType type = GetAssetTypeFromFileExtension( filePath.extension() );

		switch ( type )
		{
		case AssetType::TEXTURE:
			m_TextureManager.UnloadTexture( filePath );
			break;
		case AssetType::MODEL:
			m_ModelManager.UnloadModel( filePath );
			break;
		case AssetType::SHADER:
			m_ShaderManager.UnloadShader( filePath );
			break;
		case AssetType::MATERIAL:
			m_MaterialManager.UnloadMaterial( filePath );
			break;
		case AssetType::SCENE:
			MIKU_CORE_WARN( "Hot Reloading of Scene is not implemented yet!" );
			break;
		case AssetType::SCRIPT:
			MIKU_CORE_WARN( "Hot Reloading of Script is not implemented yet!" );
			break;
		case AssetType::NONE:
			MIKU_CORE_WARN( "Hot Reloading of this filetype is not implemented yet!" );
			break;
		default:
			MIKU_CORE_WARN( "Hot Reloading of this filetype is not implemented yet!" );
			break;
		}
	}
}
