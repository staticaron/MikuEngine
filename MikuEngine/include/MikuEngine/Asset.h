#pragma once

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

	protected:
		AssetType m_AssetType;
	};
}
