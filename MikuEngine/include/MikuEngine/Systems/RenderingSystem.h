#pragma once

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class AppLevelStuff;
}

namespace MikuEngine
{
	struct CameraData
	{
		glm::mat4 viewMatrix;
		glm::vec2 cameraResolution;
	};

	class MIKU_API RenderingSystem
	{
	public:
		static void RenderSprite( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData );
	};
}
