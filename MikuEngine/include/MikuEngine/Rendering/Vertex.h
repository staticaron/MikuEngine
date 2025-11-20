#pragma once

#include "glm/glm.hpp"

namespace MikuEngine
{
	struct Vertex
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 UV = { 0.0f, 0.0f };
	};
}
