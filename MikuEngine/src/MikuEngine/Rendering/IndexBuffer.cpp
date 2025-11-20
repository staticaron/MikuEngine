#include "Rendering/IndexBuffer.h"
#include "Application.h"

namespace MikuEngine
{
	IndexBuffer::IndexBuffer( unsigned int count, void* data ) : m_Count( count )
	{
		glCreateBuffers( 1, &m_RendererID );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_RendererID );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof( unsigned int ), data, GL_DYNAMIC_DRAW );
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers( 1, &m_RendererID );
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_RendererID );
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

	void IndexBuffer::PutData( void* data, unsigned int count )
	{
		glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, count, data );
	}
}
