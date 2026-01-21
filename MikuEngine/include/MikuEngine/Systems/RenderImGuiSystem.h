#pragma once

#include "entt/entt.hpp"

#include "Core.h"

namespace MikuEngine
{
	class AppLevelStuff;
}

namespace MikuEngine
{
	class MIKU_API RenderImGuiSystem
	{
	public:
		static void RenderImGui( entt::registry& registry, const AppLevelStuff& appLevelStuff );
	};
}
