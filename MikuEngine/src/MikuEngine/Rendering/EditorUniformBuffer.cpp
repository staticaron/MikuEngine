#include "Rendering/EditorUniformBuffer.h"

#include "glad/glad.h"

namespace MikuEngine
{
	void EditorUniformBuffer::Init( unsigned int size )
	{
		glGenBuffers( 1, &m_RendererID );
		Bind();
		glBufferData( GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW );
		UnBind();
	}
}
