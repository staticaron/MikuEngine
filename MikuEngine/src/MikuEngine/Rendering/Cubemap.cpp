#include "Rendering/Cubemap.h"

#include "glad/glad.h"
#include "stb_image/stb_image.h"

#include "Logger.h"

namespace MikuEngine
{
	void Cubemap::LoadFromFile( const std::filesystem::path& path )
	{
		glGenTextures( 1, &m_RendererID );
		glBindTexture( GL_TEXTURE_CUBE_MAP, m_RendererID );

		for ( unsigned int x = 0; x < 6; x++ )
		{
			auto fileName = path.stem();
			auto newPath = path.parent_path() / ( fileName.string() + std::to_string( x ) + path.extension().string() );

			unsigned char* data = stbi_load( newPath.c_str(), &m_Width, &m_Height, &m_Channels, 4 );

			MIKU_CORE_INFO( "Image Location : {}", newPath.c_str() );

			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + x, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

			stbi_image_free( data );
		}

		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

		glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
	}

	void Cubemap::Bind( unsigned int index ) const
	{
		glActiveTexture( GL_TEXTURE0 + index );
		glBindTexture( GL_TEXTURE_CUBE_MAP, m_RendererID );
	}

	void Cubemap::UnBind() const
	{
		glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
	}
}
