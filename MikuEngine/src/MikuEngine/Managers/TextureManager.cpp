#include "Managers/TextureManager.h"

#include <filesystem>
#include <unordered_map>

#include "Logger.h"
#include "Managers/MetaFileManager.h"
#include "Rendering/Texture.h"

namespace MikuEngine
{
	TextureManager::TextureManager()
	{
		PrepareTextureIndex();
	}

	TextureManager::~TextureManager()
	{
		for ( auto [ identifier, texture ] : m_Textures )
			texture.texture.Destroy();
	}

	void TextureManager::LoadAllTextures()
	{
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

		MIKU_CORE_DEBUG( "All Textures Loaded!" );
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
		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/textures/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_TextureIndex[ uuid ] = { uuid, file.path().string() };
		}

		MIKU_CORE_INFO( "Texture Indexing Complete!" );
	}

	const std::unordered_map<UUID, TextureIndexEntry>& TextureManager::GetTextureIndex() const
	{
		return m_TextureIndex;
	}

	std::optional<TextureContainer*> TextureManager::GetTexture( UUID textureUUID )
	{
		if ( auto existing = m_Textures.find( textureUUID ); existing != m_Textures.end() ) return &existing->second;
		return {};
	}

	std::optional<const TextureContainer*> TextureManager::GetTexture( UUID uuid ) const
	{
		if ( auto existing = m_Textures.find( uuid ); existing != m_Textures.end() ) return &existing->second;
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

	std::optional<const TextureContainer*> TextureManager::GetTextureByFilePath( const std::string& path ) const
	{
		for ( const auto [ uuid, textureIndex ] : m_TextureIndex )
			if ( textureIndex.path == path ) return { GetTexture( uuid ) };

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
}
