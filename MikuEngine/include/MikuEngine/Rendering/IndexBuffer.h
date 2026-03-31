#pragma once

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API IndexBuffer
	{
	public:
		IndexBuffer();

		void Init( unsigned int count, void* data );
		void Destroy();

		void Bind() const;
		void UnBind() const;

		unsigned int Count() const { return m_Count; }

		void PutData( void* data, unsigned int count );

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_Count = 0;
	};
}
