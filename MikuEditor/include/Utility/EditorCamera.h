#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "MikuEngine/Application.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/ICamera.h"

namespace MikuEditor
{
	class EditorCamera : public MikuEngine::ICamera
	{
	public:
		EditorCamera();
		~EditorCamera();

		glm::mat4 GetMVPFromModelMatrix( const MikuEngine::Entity& entity, glm::mat4 modelMatrix ) const override { return GetProjViewMatrix( entity ) * modelMatrix; }

		glm::mat4 GetProjViewMatrix( const MikuEngine::Entity& entity ) const override
		{
			glm::mat4 projMatrix = glm::ortho( 0.0f, MikuEngine::Application::GetDataContainer().GetViewportSize().x, MikuEngine::Application::GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
			glm::mat4 viewMatrix = GetViewMatrix( entity );

			return projMatrix * viewMatrix;
		}

		glm::mat4 GetViewMatrix( const MikuEngine::Entity& entity ) const override
		{
			const auto& cameraTransform = entity.GetReadOnlyComponent<MikuEngine::TransformComponent>();
			auto transform = glm::translate( glm::mat4( 1.0f ), cameraTransform.Position );

			auto rotationX = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			auto rotationY = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			auto rotationZ = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

			auto rotation = rotationX * rotationY * rotationZ;
			return glm::inverse( transform * rotation );
		}

	private:
	};
}
