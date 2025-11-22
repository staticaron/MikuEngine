#pragma once

#include <string>

namespace MikuEngine
{
	class Texture
	{
	public:
		Texture() = default;

		void LoadFromFile( const std::string& filepath );

		void Bind( unsigned int position ) const;
		void UnBind() const;

		void Destroy();

	private:
		unsigned int m_RendererID = 0;
		int m_Width = -1, m_Height = -1;
		int m_Channels = -1;
	};
}
