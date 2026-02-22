#pragma once

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	enum class SelectableType
	{
		ENTITY = 0,
		ASSET
	};

	struct MIKU_API SelectableItem
	{
		UUID uuid;
		SelectableType type;
	};
}
