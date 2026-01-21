#pragma once

#include "glm/glm.hpp"

#include "Core.h"

namespace MikuEngine
{
	class Entity;

	class MIKU_API ICamera
	{
	public:
		virtual glm::mat4 GetMVPFromModelMatrix( const Entity& entity, glm::mat4 modelMatrix ) const = 0;
		virtual glm::mat4 GetProjViewMatrix( const Entity& entity ) const = 0;
		virtual glm::mat4 GetViewMatrix( const Entity& entity ) const = 0;

	private:
	};
}
