#include "GameObjects/Camera.h"

#include "Application.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace MikuEngine
{
	glm::mat4 Camera::GetMVPFromModelMatrix( glm::mat4 modelMatrix ) const
	{
		return GetProjViewMatrix() * modelMatrix;
	}

	glm::mat4 Camera::GetProjViewMatrix() const
	{
		glm::mat4 projMatrix = glm::ortho( 0.0f, Application::GetDataContainer().GetViewportSize().x, Application::GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
		glm::mat4 viewMatrix = GetViewMatrix();

		return projMatrix * viewMatrix;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::mat4( 1.0f );
	}
}
