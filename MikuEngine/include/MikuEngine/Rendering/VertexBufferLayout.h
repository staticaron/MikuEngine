#pragma once

#include <vector>

#include "Core.h"
#include "glad/glad.h"

namespace MikuEngine
{
	struct MIKU_API VertexBufferElement
	{
		unsigned int type;
		unsigned int count;

		unsigned int GetSizeOfType() const
		{
			switch ( type )
			{
			case GL_FLOAT:
				return sizeof( float );
			case GL_UNSIGNED_INT:
				return sizeof( unsigned int );
			default:
				return 0;
			}
		}
	};

	class MIKU_API VertexBufferLayout
	{
	public:
		template <typename T>
		void Add( unsigned int count )
		{
			static_assert( false );
		}

		const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }

		unsigned int GetStride() const { return m_Stride; }

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};

	template <>
	inline void VertexBufferLayout::Add<float>( unsigned int count )
	{
		m_Elements.push_back( { GL_FLOAT, count } );
		m_Stride += sizeof( float ) * count;
	}

	template <>
	inline void VertexBufferLayout::Add<unsigned int>( unsigned int count )
	{
		m_Elements.push_back( { GL_UNSIGNED_INT, count } );
		m_Stride += sizeof( unsigned int ) * count;
	}
}
