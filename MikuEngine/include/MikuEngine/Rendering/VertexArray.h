#pragma once

#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Setup( const VertexBuffer& vb, const VertexBufferLayout& vbl );

		void Bind() const;
		void UnBind() const;

	private:
		unsigned int m_RendererID;
	};
}
