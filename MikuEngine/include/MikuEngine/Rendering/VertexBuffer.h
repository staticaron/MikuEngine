#pragma once

namespace MikuEngine
{
	class VertexBuffer
	{
	public:
		VertexBuffer( unsigned int size, void* data );
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;

		void PutData( void* data, unsigned int size );

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_Size = 0;
	};
}
