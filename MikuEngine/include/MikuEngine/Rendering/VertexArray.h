#pragma once

#include "Core.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	class MIKU_API VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Init();

		void Setup( const VertexBuffer& vb, const VertexBufferLayout& vbl );

		void Bind() const;
		void UnBind() const;

	private:
		unsigned int m_RendererID;
	};
}
