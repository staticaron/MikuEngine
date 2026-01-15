#pragma once

#include <string>

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API Texture
	{
	public:
		Texture( UUID uuid );

		void LoadFromFile( const std::string& filepath );

		void Bind( unsigned int position ) const;
		void UnBind() const;

		void Destroy();

		UUID GetUUID() const { return m_UUID; }
		unsigned int GetRendererID() const { return m_RendererID; }

	private:
		UUID m_UUID;
		unsigned int m_RendererID = 0;
		int m_Width = -1, m_Height = -1;
		int m_Channels = -1;
	};
}
