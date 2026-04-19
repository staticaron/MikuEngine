#pragma once

#include "glm/ext/matrix_float4x4.hpp"

namespace MikuEngine
{
	struct CameraData
	{
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;

		glm::mat4 GetViewMatrix() const { return viewMatrix; }
		glm::mat4 GetProjMatrix() const { return projMatrix; }
		glm::mat4 GetProjViewMatrix() const { return projMatrix * viewMatrix; }
	};
}
