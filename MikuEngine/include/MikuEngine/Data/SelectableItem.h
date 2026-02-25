#pragma once

#include "Core.h"
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
	};
}
