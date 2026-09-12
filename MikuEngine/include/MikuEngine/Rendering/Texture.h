#pragma once

#include <string>

#include "Core.h"
#include "IAsset.h"
#include "UUID.h"

namespace MikuEngine
{
	enum class TextureWrapMode
	{
		CLAMP,
		REPEAT
	};

	class MIKU_API Texture : IAsset
	{
	public:
		Texture() : IAsset( AssetType::TEXTURE ) {};
		Texture( UUID uuid ) : m_UUID( uuid ), IAsset( AssetType::TEXTURE ) {}
		Texture( UUID uuid, const std::filesystem::path& filepath );

		void Load( const std::filesystem::path& filepath );
		void Destroy();

		void Bind( unsigned int position ) const;
		void UnBind() const;

		unsigned int GetRendererID() const { return m_RendererID; }

		const UUID& GetUUID() const override { return m_UUID; }
		std::string GetName() const override { return m_FilePath.stem().string(); }
		const std::filesystem::path& GetPath() const override { return m_FilePath; }
		void SetPath( const std::filesystem::path& path ) override { m_FilePath = path; }

		TextureWrapMode GetWrapMode() const { return m_WrapMode; }

	private:
		UUID m_UUID;
		std::filesystem::path m_FilePath{};

		unsigned int m_RendererID = 0;
		int m_Width = -1, m_Height = -1;
		int m_Channels = -1;

		TextureWrapMode m_WrapMode = TextureWrapMode::CLAMP;
	};
}
