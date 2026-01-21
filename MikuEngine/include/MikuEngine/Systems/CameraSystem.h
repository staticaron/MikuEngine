#pragma once

#include "glm/glm.hpp"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
}

namespace MikuEngine
{
	class MIKU_API CameraSystem
	{
	public:
		static glm::mat4 GetMVPFromModelMatrix( const Entity& entity, glm::mat4 modelMatrix );
		static glm::mat4 GetProjViewMatrix( const Entity& entity );
		static glm::mat4 GetViewMatrix( const Entity& entity );
	};
}
