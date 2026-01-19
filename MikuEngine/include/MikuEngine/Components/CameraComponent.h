#pragma once

#include "glm/ext/quaternion_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Application.h"
#include "Core.h"

#include "Components/BaseComponent.h"
#include "Components/TransformComponent.h"

namespace MikuEngine
{
	class MIKU_API CameraComponent : public BaseComponent
	{
	public:
		CameraComponent() = default;
		CameraComponent( const CameraComponent& ) = default;

		CameraComponent( float zoomLevel ) : Zoom( zoomLevel ) {}

		glm::mat4 GetMVPFromModelMatrix( const entt::registry& registry, const entt::entity& entity, glm::mat4 modelMatrix ) const { return GetProjViewMatrix( registry, entity ) * modelMatrix; }

		glm::mat4 GetProjViewMatrix( const entt::registry& registry, const entt::entity& entity ) const
		{
			glm::mat4 projMatrix = glm::ortho( 0.0f, Application::GetDataContainer().GetViewportSize().x, Application::GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
			glm::mat4 viewMatrix = GetViewMatrix( registry, entity );

			return projMatrix * viewMatrix;
		}

		glm::mat4 GetViewMatrix( const entt::registry& registry, const entt::entity& entity ) const
		{
			const auto& cameraTransform = registry.get<TransformComponent>( entity );

			auto transform = glm::translate( glm::mat4( 1.0f ), cameraTransform.Position );

			auto rotationX = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			auto rotationY = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			auto rotationZ = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

			auto rotation = rotationX * rotationY * rotationZ;

			return glm::inverse( transform * rotation );
		}

	public:
		float Zoom = 1.0f;
		glm::vec2 m_AspectRatio = { 16, 9 };
	};
}
