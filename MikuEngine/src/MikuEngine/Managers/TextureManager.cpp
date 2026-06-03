#include "Managers/TextureManager.h"

#include <filesystem>
#include <unordered_map>

#include "Application.h"
#include "Logger.h"
#include "Managers/MetaFileManager.h"
#include "Rendering/Texture.h"

namespace MikuEngine
{
	TextureManager::~TextureManager()
	{
		for ( auto [ identifier, texture ] : m_Textures )
			texture.texture.Destroy();
	}

	void TextureManager::LoadAllTextures()
	{
		PrepareTextureIndex();

		LoadAllDefaultTextures();

		// LOAD TEXTURES
		for ( auto [ identifier, textureIndexEntry ] : m_TextureIndex )
		{
			if ( TextureAlreadyPresent( identifier ) )
			{
				MIKU_CORE_ERROR( "Texture with ID : {} is already loaded!", textureIndexEntry.path.stem().string() );
				continue;
			}

			Texture newTexture( identifier );
			newTexture.LoadFromFile( textureIndexEntry.path );
			m_Textures.insert( {
			    identifier, { textureIndexEntry, newTexture }
			  } );
		}

		const auto& dataContainer = Application::GetDataContainer();

		// LOAD CUBE MAPS
		Cubemap cubemap;
		cubemap.LoadFromFile( dataContainer.GetProjectAssetPath( "/textures/skybox/cubemap.jpg" ) );

		UUID uuid( "3583395674924388584" );
		m_Cubemaps[ uuid ] = CubemapContainer{ uuid, cubemap };

		MIKU_CORE_INFO( "Loaded Cubemap @ {}", m_Cubemaps[ uuid ].uuid.ToString() );

		MIKU_CORE_DEBUG( "PROJECT {} Textures Loaded!", m_Textures.size() );
	}

	void TextureManager::LoadAllDefaultTextures()
	{
		for ( auto [ identifier, textureIndexEntry ] : m_DefaultTextureIndex )
		{
			if ( TextureAlreadyPresent( identifier ) )
			{
				MIKU_CORE_ERROR( "Texture with ID : {} is already loaded!", textureIndexEntry.path.stem().string() );
				continue;
			}

			Texture newTexture( identifier );
			newTexture.LoadFromFile( textureIndexEntry.path );
			m_DefaultTextures.insert( {
			    identifier, { textureIndexEntry, newTexture }
			  } );
		}

		MIKU_CORE_DEBUG( "DEFAULT {} Textures Loaded!", m_DefaultTextures.size() );
	}

	void TextureManager::LoadTexture( const std::string& name, const std::filesystem::path& filepath )
	{
		UUID newUUID = UUID();
		Texture newTexture( newUUID );
		newTexture.LoadFromFile( filepath );

		m_Textures.insert( {
		    newTexture.GetUUID(), { { newUUID, filepath }, newTexture }
		} );

		MIKU_CORE_INFO( "Texture with ID : {} loaded!", name );
	}

	void TextureManager::PrepareTextureIndex()
	{
		// LOAD DEFAULT TEXTURES
		for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/textures/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::TEXTURE, TextureManager::GetTextureProperties( {} ) );

			if ( file.is_directory() ) continue;

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_DefaultTextureIndex[ uuid ] = { uuid, file.path().string() };
		}

		const auto& dataContainer = Application::GetDataContainer();

		// LOAD PROJECT TEXTURES
		for ( auto& file : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectAssetPath( "/textures/" ) ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::TEXTURE, TextureManager::GetTextureProperties( {} ) );

			if ( file.is_directory() ) continue;

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_TextureIndex[ uuid ] = { uuid, file.path().string() };
		}

		MIKU_CORE_INFO( "Texture Indexing Complete!" );
	}

	const std::unordered_map<UUID, TextureIndexEntry>& TextureManager::GetTextureIndex() const
	{
		return m_TextureIndex;
	}

	TextureContainer* TextureManager::GetTextureOrDefault( UUID textureUUID )
	{
		if ( auto existing = m_Textures.find( textureUUID ); existing != m_Textures.end() ) return &existing->second;
		if ( auto existing = m_DefaultTextures.find( textureUUID ); existing != m_DefaultTextures.end() ) return &existing->second;

		return GetDefaultTextureByName( "default_tex" );
	}

	const TextureContainer* TextureManager::GetTextureOrDefault( UUID uuid ) const
	{
		if ( auto existing = m_Textures.find( uuid ); existing != m_Textures.end() ) return &existing->second;
		if ( auto existing = m_DefaultTextures.find( uuid ); existing != m_DefaultTextures.end() ) return &existing->second;

		return GetDefaultTextureByName( "default_tex" );
	}

	std::optional<TextureContainer*> TextureManager::GetTexture( UUID textureUUID )
	{
		if ( auto existing = m_Textures.find( textureUUID ); existing != m_Textures.end() ) return &existing->second;
		if ( auto existing = m_DefaultTextures.find( textureUUID ); existing != m_DefaultTextures.end() ) return &existing->second;

		return {};
	}

	std::optional<const TextureContainer*> TextureManager::GetTexture( UUID textureUUID ) const
	{
		if ( auto existing = m_Textures.find( textureUUID ); existing != m_Textures.end() ) return &existing->second;
		if ( auto existing = m_DefaultTextures.find( textureUUID ); existing != m_DefaultTextures.end() ) return &existing->second;

		return {};
	}

	std::optional<CubemapContainer*> TextureManager::GetCubemap( UUID cubemapUUID )
	{
		if ( auto existing = m_Cubemaps.find( cubemapUUID ); existing != m_Cubemaps.end() ) return &existing->second;
		return {};
	}

	std::optional<const CubemapContainer*> TextureManager::GetCubemap( UUID cubemapUUID ) const
	{
		if ( auto existing = m_Cubemaps.find( cubemapUUID ); existing != m_Cubemaps.end() ) return &existing->second;
		return {};
	}

	std::optional<const TextureContainer*> TextureManager::GetTextureByName( const std::string& filename ) const
	{
		for ( const auto& [ uuid, textureContainer ] : m_Textures )
		{
			if ( textureContainer.GetName() == filename ) return &m_Textures.at( uuid );
		}

		return {};
	}

	TextureContainer* TextureManager::GetDefaultTextureByName( const std::string& name )
	{
		for ( auto& defaultTexture : m_DefaultTextures )
		{
			if ( defaultTexture.second.GetName() == name ) return &defaultTexture.second;
		}

		MIKU_ASSERT( false, "The requested default texture by name" );
	}

	const TextureContainer* TextureManager::GetDefaultTextureByName( const std::string& name ) const
	{
		for ( const auto& defaultTexture : m_DefaultTextures )
		{
			if ( defaultTexture.second.GetName() == name ) return &defaultTexture.second;
		}

		MIKU_ASSERT( false, "The requested default texture by name" );
	}

	std::optional<const TextureContainer*> TextureManager::GetTextureByFilePath( const std::string& path ) const
	{
		for ( const auto [ uuid, textureIndex ] : m_TextureIndex )
			if ( textureIndex.path == path ) return { GetTextureOrDefault( uuid ) };

		return {};
	}

	bool TextureManager::TextureExists( const UUID& uuid ) const
	{
		auto exists = m_Textures.find( uuid );
		return exists != m_Textures.end();
	}

	const std::unordered_map<UUID, TextureContainer>& TextureManager::GetAllLoadedTextures() const
	{
		return m_Textures;
	}

	const std::unordered_map<UUID, TextureContainer>& TextureManager::GetAllDefaultTextures() const
	{
		return m_DefaultTextures;
	}

	const std::filesystem::path& TextureManager::GetFilePathFromUUID( const UUID& uuid )
	{
		if ( auto existing = m_Textures.find( uuid ); existing != m_Textures.end() )
		{
			return existing->second.index.path;
		}

		MIKU_ASSERT( false, "This texture is not loaded!" );
	}

	void TextureManager::RenameAssetCleanup( const UUID& uuid, const std::string& newName )
	{
		if ( auto existing = m_Textures.find( uuid ); existing != m_Textures.end() )
		{
			existing->second.SetName( newName );
		}
	}

	void TextureManager::DeleteAssetCleanup( const UUID& uuid ) {}

	bool TextureManager::TextureAlreadyPresent( UUID identifier ) const
	{
		auto existing = m_Textures.find( identifier );

		if ( existing != m_Textures.end() ) return true;

		return false;
	}

	YAML::Node TextureManager::GetTextureProperties( std::optional<Texture*> texture )
	{
		YAML::Node properties;

		// return the default texture meta file properties
		if ( !texture.has_value() )
		{
			properties[ "wrap" ] = "clamp";
			return properties;
		}

		// return the properties of the texture provided
		switch ( texture.value()->GetWrapMode() )
		{
		case TextureWrapMode::REPEAT:
			properties[ "wrap" ] = "repeat";
			break;
		case TextureWrapMode::CLAMP:
			properties[ "wrap" ] = "clamp";
			break;
		default:
			properties[ "wrap" ] = "clamp";
		}

		return properties;
	}
}
