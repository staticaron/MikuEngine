#include "Rendering/Texture.h"

#include <string>

#include "glad/glad.h"
#include "stb_image/stb_image.h"

namespace MikuEngine
{
	Texture::Texture( UUID uuid ) : m_UUID( uuid ), Asset( AssetType::TEXTURE ) {}

	void Texture::LoadFromFile( const std::string& filepath )
	{
		stbi_set_flip_vertically_on_load( true );

		unsigned char* data = stbi_load( filepath.c_str(), &m_Width, &m_Height, &m_Channels, 4 );

		glGenTextures( 1, &m_RendererID );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_RendererID );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

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
}
