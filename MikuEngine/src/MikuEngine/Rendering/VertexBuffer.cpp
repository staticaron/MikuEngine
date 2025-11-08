#include "Rendering/VertexBuffer.h"
#include "glad/glad.h"

namespace MikuEngine
{
	VertexBuffer::VertexBuffer( unsigned int size, void* data )
	{
		glCreateBuffers( 1, &m_RendererID );
		glBindBuffer( GL_ARRAY_BUFFER, m_RendererID );
		glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers( 1, &m_RendererID );
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, m_RendererID );
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void VertexBuffer::PutData( void* data, unsigned int size ) {}
}
