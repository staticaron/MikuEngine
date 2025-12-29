#include "Rendering/VertexBuffer.h"

#include "glad/glad.h"
#include "Logger.h"

namespace MikuEngine
{
	VertexBuffer::VertexBuffer() {}

	VertexBuffer::~VertexBuffer()
	{
		MIKU_CORE_WARN( "Vertex Buffer deleted! ID : {}", m_RendererID );
		glDeleteBuffers( 1, &m_RendererID );
	}

	void VertexBuffer::Init( unsigned int size, void* data )
	{
		m_Size = size;

		glCreateBuffers( 1, &m_RendererID );
		glBindBuffer( GL_ARRAY_BUFFER, m_RendererID );
		glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );

		MIKU_CORE_INFO( "Vertex Buffer Created! ID : {}", m_RendererID );
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, m_RendererID );
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void VertexBuffer::PutData( void* data, unsigned int size )
	{
		Bind();
		glBufferSubData( GL_ARRAY_BUFFER, 0, size, data );
	}
}
