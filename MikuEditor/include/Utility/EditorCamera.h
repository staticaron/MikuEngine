#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "MikuEngine/Application.h"
#include "MikuEngine/Input/Input.h"

namespace MikuEditor
{
	class EditorCamera
	{
	public:
		EditorCamera() = default;
		~EditorCamera() = default;

		glm::mat4 GetMVPFromModelMatrix( glm::mat4 modelMatrix ) const { return GetProjViewMatrix() * modelMatrix; }

		glm::mat4 GetProjViewMatrix() const { return GetProjMatrix() * GetViewMatrix(); }

		glm::mat4 GetProjMatrix() const
		{
			if ( m_IsPerspective == false )
				return glm::ortho( 0.0f, MikuEngine::Application::GetDataContainer().GetViewportSize().x, MikuEngine::Application::GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
			else
				return glm::perspective( glm::pi<float>() * 0.5f, MikuEngine::Application::GetDataContainer().GetViewportAspectRatio(), 0.0f, 1000.0f );
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

		// Move the Editor Viewport Camera
		void Translate()
		{
			const auto& [ xAxis, yAxis ] = MikuEngine::Input::GetAxisRaw();
			Position = { Position.x + xAxis, Position.y + yAxis, Position.z };
		}

	private:
		glm::vec3 Position{ 1.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		bool m_IsPerspective = true;

		float m_Zoom;
	};
}
