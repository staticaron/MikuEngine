#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Core.h"

#include "Components/BaseComponent.h"

namespace MikuEngine
{
	class MIKU_API TransformComponent : public BaseComponent
	{
	public:
		TransformComponent() = default;
		TransformComponent( const TransformComponent& ) = default;
		TransformComponent( const glm::vec3& transform, const glm::vec3& rotation, const glm::vec3& scale ) : Position( transform ), Rotation( rotation ), Scale( scale ) {};

		glm::vec3 Position{ 1.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		glm::mat4 GetModelMatrix() const
		{
			auto transformMat = glm::translate( glm::mat4( 1.0f ), Position );

			auto rotationX = glm::rotate( glm::mat4( 1.0f ), Rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			auto rotationY = glm::rotate( glm::mat4( 1.0f ), Rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			auto rotationZ = glm::rotate( glm::mat4( 1.0f ), Rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			auto rotationMat = rotationX * rotationY * rotationZ;

			auto scaleMat = glm::scale( glm::mat4( 1.0f ), Scale );

			return transformMat * rotationMat * scaleMat;
		}

		glm::quat GetQuaternionRotation() const { return glm::quat( glm::vec3{ Rotation.x, Rotation.y, Rotation.z } ); }

		glm::vec3 GetForward() const { return glm::rotate( GetQuaternionRotation(), glm::vec3( 0.0f, 0.0f, 1.0f ) ); }
	};
}
