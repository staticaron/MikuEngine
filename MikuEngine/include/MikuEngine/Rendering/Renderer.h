#pragma once

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

		const Quad& GetQuad() const { return m_QuadPrimitive; }
		const Cube& GetCube() const { return m_CubePrimitive; }

	private:
		Quad m_QuadPrimitive;
		Cube m_CubePrimitive;
	};
}
