#include "Rendering/Renderer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	void Renderer::Init()
	{
		m_QuadPrimitive.Init();
	}

	void Renderer::Draw( const VertexArray& va, const IndexBuffer& ib, const Shader& shader ) const
	{
		va.Bind();
		ib.Bind();
		shader.Bind();

		glDrawElements( GL_TRIANGLES, ib.Count(), GL_UNSIGNED_INT, nullptr );
	}
}
