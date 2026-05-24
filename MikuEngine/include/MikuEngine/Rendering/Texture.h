#pragma once

#include <string>

#include "Asset.h"
#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	enum class TextureWrapMode
	{
		CLAMP,
		REPEAT
	};

	class MIKU_API Texture : Asset
	{
	public:
		Texture() : Asset( AssetType::TEXTURE ) {};
		Texture( UUID uuid ) : m_UUID( uuid ), Asset( AssetType::TEXTURE ) {}

		void LoadFromFile( const std::filesystem::path& filepath );

		void Bind( unsigned int position ) const;
		void UnBind() const;

		void Destroy();

		UUID GetUUID() const { return m_UUID; }
		unsigned int GetRendererID() const { return m_RendererID; }
		TextureWrapMode GetWrapMode() const { return m_WrapMode; }

		const std::filesystem::path& GetPath() const override;
		std::string GetName() const override;
		void SetName( const std::string& newName ) override;

		void DeleteAsset() override {}

	private:
		UUID m_UUID;

		unsigned int m_RendererID = 0;
		int m_Width = -1, m_Height = -1;
		int m_Channels = -1;

		TextureWrapMode m_WrapMode = TextureWrapMode::CLAMP;
	};
}
