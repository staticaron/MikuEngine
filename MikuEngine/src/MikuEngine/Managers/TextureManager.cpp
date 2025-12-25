#include "Managers/TextureManager.h"

#include <cassert>
#include <string_view>

#include "Logger.h"
#include "Rendering/Texture.h"

namespace MikuEngine
{
	TextureManager::TextureManager() {}

	TextureManager::~TextureManager()
	{
		for ( auto [ identifier, texture ] : m_Textures )
		{
			texture.Destroy();
		}
	}

	void TextureManager::LoadAllTextures()
	{
		for ( auto [ identifier, filepath ] : m_TexturesToLoad )
		{
			if ( TextureAlreadyPresent( identifier ) )
			{
				MIKU_ERROR( "Texture with ID : {} is already loaded!", identifier );
				continue;
			}

			Texture newTexture;
			newTexture.LoadFromFile( filepath );
			m_Textures[ identifier ] = newTexture;

			MIKU_INFO( "Texture with ID : {} loaded!", identifier );
		}
	}

	void TextureManager::LoadTexture( const std::string_view identifier, const std::string& filepath )
	{
		if ( TextureAlreadyPresent( identifier ) )
		{
			MIKU_ERROR( "Texture with ID : {} is already loaded!", identifier );
			return;
		}

		Texture newTexture;
		newTexture.LoadFromFile( filepath );
		m_Textures[ identifier ] = newTexture;

		MIKU_INFO( "Texture with ID : {} loaded!", identifier );
	}

	bool TextureManager::TextureAlreadyPresent( std::string_view identifier )
	{
		auto existing = m_Textures.find( identifier );

		if ( existing != m_Textures.end() ) return true;

		return false;
	}
}
