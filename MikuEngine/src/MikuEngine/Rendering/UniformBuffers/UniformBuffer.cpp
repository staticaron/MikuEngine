#include "Rendering/UniformBuffers/UniformBuffer.h"

#include "glad/glad.h"

namespace MikuEngine
{
	void UniformBuffer::Destroy()
	{
		glDeleteBuffers( 1, &m_RendererID );
	}

	void UniformBuffer::Bind() const
	{
		glBindBuffer( GL_UNIFORM_BUFFER, m_RendererID );
		glBindBufferBase( GL_UNIFORM_BUFFER, 0, m_RendererID );
	}

	void UniformBuffer::UnBind() const
	{
		glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	}

	void UniformBuffer::PutData( void* data, unsigned int size ) const
	{
		Bind();
		glBufferSubData( GL_UNIFORM_BUFFER, 0, size, data );
		UnBind();
	}
}
