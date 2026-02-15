#pragma once

#include "Rendering/IndexBuffer.h"
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

		const Quad& GetQuad() const { return m_Quad; }

	private:
		Quad m_Quad;
	};
}
