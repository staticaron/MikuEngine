#pragma once

#include "Managers/UniformBufferManager.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Primitives/Cube.h"
#include "Rendering/Primitives/Quad.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"

namespace MikuEngine
{
	class Renderer
	{
	public:
		void Init();

		void Draw( const VertexArray& va, const IndexBuffer& ib, const Shader& shader ) const;

		static void ClearColor( glm::vec4 color = { 0.1f, 0.1f, 0.1f, 1.0f } );

		static void DisableWriteToDepthBuffer();
		static void EnableWriteToDepthBuffer();

		const Quad& GetQuad() const { return m_QuadPrimitive; }
		const Cube& GetCube() const { return m_CubePrimitive; }
		UniformBufferManager& GetUniformBufferManager() { return m_UniformBufferManager; }

	private:
		Quad m_QuadPrimitive;
		Cube m_CubePrimitive;

		UniformBufferManager m_UniformBufferManager;
	};
}
