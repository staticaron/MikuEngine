#include "Managers/TextureManager.h"

#include <filesystem>

#include "Logger.h"
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
			texture.Destroy();
	}

	void TextureManager::LoadAllTextures()
	{
		for ( auto [ identifier, textureIndexEntry ] : m_TextureIndex )
		{
			if ( TextureAlreadyPresent( identifier ) )
			{
				MIKU_ERROR( "Texture with ID : {} is already loaded!", textureIndexEntry.name );
				continue;
			}

			Texture newTexture( identifier );
			newTexture.LoadFromFile( textureIndexEntry.path );
			m_Textures.insert( { identifier, newTexture } );

			MIKU_INFO( "Texture with ID : {} loaded!", textureIndexEntry.name );
		}
	}

	void TextureManager::LoadTexture( const std::string& name, const std::string& filepath )
	{
		Texture newTexture(( UUID() ));
		newTexture.LoadFromFile( filepath );

		m_Textures.insert( { newTexture.GetUUID(), newTexture } );

		MIKU_INFO( "Texture with ID : {} loaded!", name );
	}

	void TextureManager::PrepareTextureIndex()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "textures/" ) )
		{
			UUID uuid;
			m_TextureIndex[ uuid ] = { uuid, file.path().stem().string(), file.path().string() };
		}
	}

	const std::unordered_map<UUID, TextureIndexEntry>& TextureManager::GetTextureIndex() const
	{
		return m_TextureIndex;
	}

	const Texture& TextureManager::GetTexture( UUID identifier ) const
	{
		auto existing = m_Textures.find( identifier );

		// Make sure texture is loaded first before it is returned
		MIKU_ASSERT( existing != m_Textures.end(), "Texture not loaded!" );

		return existing->second;
	}

	const Texture& TextureManager::GetTexture( const std::string& filename ) const
	{
		for ( const auto& [ uuid, textureIndexEntry ] : m_TextureIndex )
		{
			if ( textureIndexEntry.name == filename )
			{
				return m_Textures.at( uuid );
			}
		}

		// Make sure texture is loaded first before it is returned
		MIKU_ASSERT( false, "Requested Texture is not loaded!" );
	}

	std::string TextureManager::GetTextureName( UUID identifier ) const
	{
		auto existing = m_TextureIndex.find( identifier );
		return existing->second.name;
	}

	bool TextureManager::TextureAlreadyPresent( UUID identifier ) const
	{
		auto existing = m_Textures.find( identifier );

		if ( existing != m_Textures.end() ) return true;

		return false;
	}
}
