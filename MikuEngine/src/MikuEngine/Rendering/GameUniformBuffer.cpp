#include "Rendering/GameUniformBuffer.h"

#include "glad/glad.h"

namespace MikuEngine
{
	void GameUniformBuffer::Init( unsigned int size )
	{
		glGenBuffers( 1, &m_RendererID );
		Bind();
		glBufferData( GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW );
		UnBind();
		glBindBufferBase( GL_UNIFORM_BUFFER, 0, m_RendererID );
	}
}
