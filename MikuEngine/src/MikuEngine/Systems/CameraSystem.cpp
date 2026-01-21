#include "Systems/CameraSystem.h"

#include "Application.h"
#include "Entity.h"

namespace MikuEngine
{
	glm::mat4 CameraSystem::GetMVPFromModelMatrix( const Entity& cameraEntity, glm::mat4 modelMatrix )
	{
		return GetProjViewMatrix( cameraEntity ) * modelMatrix;
	}

	glm::mat4 CameraSystem::GetProjViewMatrix( const Entity& cameraEntity )
	{
		glm::mat4 projMatrix = glm::ortho( 0.0f, Application::GetDataContainer().GetViewportSize().x, Application::GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
		glm::mat4 viewMatrix = GetViewMatrix( cameraEntity );

		return projMatrix * viewMatrix;
	}

	glm::mat4 CameraSystem::GetViewMatrix( const Entity& cameraEntity )
	{
		const auto& cameraTransform = cameraEntity.GetReadOnlyComponent<TransformComponent>();

		auto transform = glm::translate( glm::mat4( 1.0f ), cameraTransform.Position );

		auto rotationX = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		auto rotationY = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		auto rotationZ = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		auto rotation = rotationX * rotationY * rotationZ;

		return glm::inverse( transform * rotation );
	}
}
