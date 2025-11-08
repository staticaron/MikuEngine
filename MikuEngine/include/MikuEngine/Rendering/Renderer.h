#pragma once

#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"

namespace MikuEngine
{
	class Renderer
	{
	public:
		void Draw( const VertexArray& va, const IndexBuffer& ib, const Shader& shader );

	private:
	};
}
