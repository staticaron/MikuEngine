#pragma once

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void Init( unsigned int size, void* data );

		void Bind() const;
		void UnBind() const;

		void PutData( void* data, unsigned int size );

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_Size = 0;
	};
}
