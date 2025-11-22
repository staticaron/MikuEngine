#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace MikuEngine
{
	class MIKU_API Camera
	{
	public:
		Camera() = default;
		~Camera() = default;

		glm::mat4 GetMVPFromModelMatrix( glm::mat4 modelMatrix ) const;
		glm::mat4 GetProjViewMatrix() const;
		glm::mat4 GetViewMatrix() const;

		void SetPosition( glm::vec3 position );
		void Translate( glm::vec3 deltaPos );

		void SetRotation( glm::vec3 rotation );
		void RotateBy( glm::vec3 deltaRot );

		void SetZoom( float zoom );
		void ZoomBy( float deltaZoom );

	private:
	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 1.0f, 0.0f, 0.0f };
		float m_Zoom = 1.0f;
	};
}
