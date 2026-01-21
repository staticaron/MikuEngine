#pragma once

#include "entt/entt.hpp"

#include "Core.h"

#include "AppLevelStuff.h"

namespace MikuEngine
{
	class Scene;
}

namespace MikuEngine
{
	class MIKU_API RenderingSystem
	{
	public:
		static void RenderSprite( const Scene& scene, AppLevelStuff& appLevelStuff );
	};
}
