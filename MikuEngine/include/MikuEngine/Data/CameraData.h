#pragma once

#include "glm/glm.hpp"

namespace MikuEngine
{
	struct CameraData
	{
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;

		glm::mat4 GetProjViewMatrix() const { return projMatrix * viewMatrix; }
	};
}
