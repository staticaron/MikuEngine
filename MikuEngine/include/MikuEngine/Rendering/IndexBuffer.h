#pragma once

namespace MikuEngine
{
	class IndexBuffer
	{
	public:
		IndexBuffer( unsigned int count, void* data );
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		unsigned int Count() const
		{
			return m_Count;
		}

		void PutData();

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_Count = 0;
	};
}
