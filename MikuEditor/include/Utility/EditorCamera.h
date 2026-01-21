#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "MikuEngine/Application.h"

namespace MikuEditor
{
	class EditorCamera
	{
	public:
		EditorCamera() = default;
		~EditorCamera() = default;

		glm::mat4 GetMVPFromModelMatrix( glm::mat4 modelMatrix ) const { return GetProjViewMatrix() * modelMatrix; }

		glm::mat4 GetProjViewMatrix() const
		{
			glm::mat4 projMatrix = glm::ortho( 0.0f, MikuEngine::Application::GetDataContainer().GetViewportSize().x, MikuEngine::Application::GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
			glm::mat4 viewMatrix = GetViewMatrix();

			return projMatrix * viewMatrix;
		}

		glm::mat4 GetViewMatrix() const
		{
			auto transform = glm::translate( glm::mat4( 1.0f ), Position );

			auto rotationX = glm::rotate( glm::mat4( 1.0f ), Rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			auto rotationY = glm::rotate( glm::mat4( 1.0f ), Rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			auto rotationZ = glm::rotate( glm::mat4( 1.0f ), Rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

			auto rotation = rotationX * rotationY * rotationZ;
			return glm::inverse( transform * rotation );
		}

	private:
		glm::vec3 Position{ 1.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		float m_Zoom;
	};
}
