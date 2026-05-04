#pragma once

#include <optional>

#include "BaseComponent.h"
#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API SkyboxComponent : public BaseComponent
	{
	public:
		std::optional<UUID> ModelIdentifier;
		std::optional<UUID> MaterialIdentifier;
	};
}
