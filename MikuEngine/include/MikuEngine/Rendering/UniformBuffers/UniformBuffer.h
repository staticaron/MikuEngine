#pragma once

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API UniformBuffer
	{
	public:
		virtual void Init( unsigned int size ) = 0;

		void Destroy();

		void Bind() const;
		void UnBind() const;

		unsigned int GetRendererID() const { return m_RendererID; }

		void PutData( void* data, unsigned int size ) const;

	protected:
		unsigned int m_RendererID;
	};
}
