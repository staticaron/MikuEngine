#include "Rendering/VertexArray.h"

#include "Logger.h"

namespace MikuEngine
{
	VertexArray::VertexArray() {}

	VertexArray::~VertexArray()
	{
		MIKU_CORE_WARN( "Vertex Array deleted! ID : {}", m_RendererID );
		glDeleteBuffers( 1, &m_RendererID );
	}

	void VertexArray::Init()
	{
		glGenVertexArrays( 1, &m_RendererID );
		MIKU_CORE_INFO( "Vertex Array Created! ID : {}", m_RendererID );
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray( m_RendererID );
	}

	void VertexArray::UnBind() const
	{
		glBindVertexArray( 0 );
	}

	void VertexArray::Setup( const VertexBuffer& vb, const VertexBufferLayout& vbl )
	{
		Bind();
		vb.Bind();

		unsigned int offset = 0;
		for ( int x = 0; x < vbl.GetElements().size(); x++ )
		{
			const VertexBufferElement& element = vbl.GetElements().at( x );

			glEnableVertexAttribArray( x );
			glVertexAttribPointer( x, element.count, element.type, GL_FALSE, vbl.GetStride(), ( const void* )offset );

			offset += element.count * element.GetSizeOfType();
		}
	}
}
