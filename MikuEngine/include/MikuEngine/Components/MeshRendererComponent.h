#pragma once

#include "Components.h"
#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API MeshRendererComponent : public BaseComponent
	{
		std::optional<UUID> ModelIdentifier;
		std::optional<UUID> MaterialIdentifier;
	};
}
