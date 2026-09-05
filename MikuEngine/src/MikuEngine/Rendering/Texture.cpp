#include "Rendering/Texture.h"

#include <string>

#include "glad/glad.h"
#include "stb_image/stb_image.h"

#include "Application.h"
#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	Texture::Texture( UUID uuid, const std::filesystem::path& filepath ) : Asset( AssetType::TEXTURE ), m_UUID( uuid )
	{
		Load( filepath );
	}

	void Texture::Load( const std::filesystem::path& filepath )
	{
		m_FilePath = filepath;

		stbi_set_flip_vertically_on_load( true );

		unsigned char* data = stbi_load( filepath.c_str(), &m_Width, &m_Height, &m_Channels, 4 );

		glGenTextures( 1, &m_RendererID );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_RendererID );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		std::optional<YAML::Node> metaNode = MetaFileManager::GetMetaFileNode( filepath );
		if ( metaNode.has_value() )
		{
			std::string wrapMode = metaNode.value()[ "properties" ][ "wrap" ].as<std::string>();

			if ( wrapMode == "repeat" )
			{
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			}
			else if ( wrapMode == "clamp" )
			{
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			}
		}

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	void Texture::Bind( unsigned int position ) const
	{
		glActiveTexture( GL_TEXTURE0 + position );
		glBindTexture( GL_TEXTURE_2D, m_RendererID );
	}

	void Texture::UnBind() const
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	void Texture::Destroy()
	{
		glDeleteTextures( 1, &m_RendererID );
	}

	std::string Texture::GetName() const
	{
		return GetPath().stem().string();
	}

	void Texture::SetName( const std::string& newName )
	{
		Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().AddToRenameQueue( m_UUID, newName );
	}

	void Texture::DeleteAsset()
	{
		Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().AddToDeleteQueue( m_UUID );
	}
}
