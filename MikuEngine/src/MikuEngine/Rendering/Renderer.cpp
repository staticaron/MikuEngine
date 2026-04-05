#include "Rendering/Renderer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	void Renderer::Init()
	{
		m_QuadPrimitive.Init();
		m_UniformBufferManager.Init();
	}

	void Renderer::Draw( const VertexArray& va, const IndexBuffer& ib, const Shader& shader ) const
	{
		va.Bind();
		ib.Bind();
		shader.Bind();

		glDrawElements( GL_TRIANGLES, ib.Count(), GL_UNSIGNED_INT, nullptr );
	}

	void Renderer::ClearColor( glm::vec4 color )
	{
		glClearColor( color.x, color.y, color.z, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}
}
