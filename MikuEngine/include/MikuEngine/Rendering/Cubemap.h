#pragma once

#include <filesystem>

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API Cubemap
	{
	public:
		void LoadFromFile( const std::filesystem::path& path );

		void Bind( unsigned int index ) const;
		void UnBind() const;

	private:
		unsigned int m_RendererID;

		int m_Width, m_Height;
		int m_Channels;
	};
}
