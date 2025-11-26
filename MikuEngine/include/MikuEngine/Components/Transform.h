#pragma once

#include "glm/glm.hpp"

namespace MikuEngine
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent( const TransformComponent& ) = default;
		TransformComponent( const glm::vec3& transform, const glm::vec3& rotation, float zoom ) : Transform( transform ), Rotation( rotation ), Zoom( zoom ) {};

		glm::vec3 Transform{ 1.0f };
		glm::vec3 Rotation{ 0.0f };
		float Zoom{ 1.0f };
	};
}
