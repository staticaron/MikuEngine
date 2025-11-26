#pragma once

#include "entt/entt.hpp"

#include "Core.h"

#include "AppLevelStuff.h"

namespace MikuEngine
{
	class MIKU_API RenderingSystem
	{
	public:
		static void RenderSprite( const entt::registry& registry, AppLevelStuff& appLevelStuff );
	};
}
