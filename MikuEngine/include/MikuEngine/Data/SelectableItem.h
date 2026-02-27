#pragma once

#include <optional>

#include "Core.h"
#include "Data/AssetType.h"
#include "UUID.h"

namespace MikuEngine
{
	enum class SelectableType
	{
		NONE = 0,
		ENTITY,
		ASSET
	};

	struct MIKU_API SelectableItem
	{
		UUID uuid;
		SelectableType type;
		std::optional<AssetType> assetType;
	};
}
