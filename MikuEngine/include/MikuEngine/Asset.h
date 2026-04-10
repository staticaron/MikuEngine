#pragma once

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

		virtual void RenderInspectorImGui() = 0;
		virtual void DeleteAsset() = 0;

	protected:
		void RenderBaseImGui( std::string& name );

	protected:
		AssetType m_AssetType;
	};
}
