#include "Rendering/VertexArray.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	VertexArray::VertexArray()
	{
		glCreateBuffers( 1, &m_RendererID );
		glBindBuffer( GL_VERTEX_ARRAY, m_RendererID );
	}

	VertexArray::~VertexArray()
	{
		glDeleteBuffers( 1, &m_RendererID );
	}

	void VertexArray::Bind() const
	{
		glBindBuffer( GL_VERTEX_ARRAY, m_RendererID );
	}

	void VertexArray::UnBind() const
	{
		glBindBuffer( GL_VERTEX_ARRAY, 0 );
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
