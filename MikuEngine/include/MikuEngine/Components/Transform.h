#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API TransformComponent
	{
		TransformComponent() = default;
		TransformComponent( const TransformComponent& ) = default;
		TransformComponent( const glm::vec3& transform, const glm::vec3& rotation, const glm::vec3& scale ) : Position( transform ), Rotation( rotation ), Scale( scale ) {};

		glm::vec3 Position{ 1.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		glm::mat4 GetModelMatrix() const
		{
			auto transformMat = glm::translate( glm::mat4( 1.0f ), Position );
			auto rotationMat = glm::rotate( glm::mat4( 1.0f ), 0.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			auto scaleMat = glm::scale( glm::mat4( 1.0f ), Scale );

			return transformMat * rotationMat * scaleMat;
		}
	};
}
