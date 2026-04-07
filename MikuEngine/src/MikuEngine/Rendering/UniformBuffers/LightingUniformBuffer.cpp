#include "Rendering/UniformBuffers/LightingUniformBuffer.h"

#include "glad/glad.h"

namespace MikuEngine
{
	void LightingUniformBuffer::Init( unsigned int bindIndex, unsigned int size )
	{
		m_BindIndex = bindIndex;

		glGenBuffers( 1, &m_RendererID );

		Bind();
		glBufferData( GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW );
		UnBind();

		glBindBufferBase( GL_UNIFORM_BUFFER, bindIndex, m_RendererID );
	}
}
