#pragma once

#include "Components.h"
#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API MeshRendererComponent : BaseComponent
	{
		std::optional<UUID> Model;
		std::optional<UUID> Material;
	};
}
