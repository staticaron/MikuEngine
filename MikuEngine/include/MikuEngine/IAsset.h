#pragma once

#include <filesystem>
#include <string>

#include "Core.h"
#include "Data/AssetType.h"

namespace MikuEngine
{
	class UUID;
}

namespace MikuEngine
{
	class MIKU_API IAsset
	{
	public:
		IAsset( AssetType assetType ) : m_AssetType( assetType ) {}
		const AssetType& GetAssetType() const { return m_AssetType; }

		virtual const UUID& GetUUID() const = 0;
		virtual std::string GetName() const = 0;
		virtual const std::filesystem::path& GetPath() const = 0;
		virtual void SetPath( const std::filesystem::path& path ) = 0;

	protected:
		AssetType m_AssetType;
	};
}
