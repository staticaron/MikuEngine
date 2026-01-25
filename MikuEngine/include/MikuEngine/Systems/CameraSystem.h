#pragma once

#include "glm/glm.hpp"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
	class CameraComponent;
}

namespace MikuEngine
{
	class MIKU_API CameraSystem
	{
	public:
		static glm::mat4 GetMVPFromModelMatrix( const Entity& entity, const CameraComponent& cameraComponent, glm::mat4 modelMatrix );
		static glm::mat4 GetProjViewMatrix( const Entity& entity, const CameraComponent& cameraComponent );
		static glm::mat4 GetViewMatrix( const Entity& entity );

		static void CameraComponentRenderImGui( CameraComponent& cameraComponent );
	};
}
