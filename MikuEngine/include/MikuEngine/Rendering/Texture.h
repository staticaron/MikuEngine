#pragma once

#include <string>

#include "Asset.h"
#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API Texture : Asset
	{
	public:
		Texture() : Asset( AssetType::TEXTURE ) {};
		Texture( UUID uuid ) : m_UUID( uuid ), Asset( AssetType::TEXTURE ) {}

		void LoadFromFile( const std::string& filepath );

		void Bind( unsigned int position ) const;
		void UnBind() const;

		void Destroy();

		UUID GetUUID() const { return m_UUID; }
		unsigned int GetRendererID() const { return m_RendererID; }

		void RenderInspectorImGui() override {}

	private:
		UUID m_UUID;
		unsigned int m_RendererID = 0;
		int m_Width = -1, m_Height = -1;
		int m_Channels = -1;
	};
}
