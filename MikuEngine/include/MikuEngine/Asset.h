#pragma once

#include <filesystem>
#include <string>

#include "Core.h"
#include "Data/AssetType.h"

namespace MikuEngine
{
	class MIKU_API Asset
	{
	public:
		Asset( AssetType assetType ) : m_AssetType( assetType ) {}
		const AssetType& GetAssetType() const { return m_AssetType; }

		virtual void DeleteAsset() = 0;

		virtual std::string GetName() const = 0;
		virtual void SetName( const std::string& newName ) = 0;

		virtual const std::filesystem::path& GetPath() const = 0;

	protected:
		AssetType m_AssetType;
	};
}
