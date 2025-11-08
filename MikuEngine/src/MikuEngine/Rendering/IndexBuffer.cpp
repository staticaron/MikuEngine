#include "Rendering/IndexBuffer.h"
#include "Application.h"

namespace MikuEngine
{
	IndexBuffer::IndexBuffer( unsigned int count, void* data ) : m_Count( count )
	{
		glCreateBuffers( 1, &m_RendererID );
		glBindBuffer( GL_ARRAY_BUFFER, m_RendererID );
		glBufferData( GL_ARRAY_BUFFER, m_Count * sizeof( float ), data, GL_DYNAMIC_DRAW );
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_RendererID );
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

	void IndexBuffer::PutData()
	{
	}
}
