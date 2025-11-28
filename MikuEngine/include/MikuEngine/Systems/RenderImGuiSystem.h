#pragma once

#include "entt/entt.hpp"

#include "Core.h"

#include "AppLevelStuff.h"

namespace MikuEngine
{
	class MIKU_API RenderImGuiSystem
	{
	public:
		static void RenderImGui( entt::registry& registry, const AppLevelStuff& appLevelStuff );
	};
}
