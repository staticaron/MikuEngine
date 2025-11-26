#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace MikuEngine
{
	struct MIKU_API Vertex
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 UV = { 0.0f, 0.0f };
	};
}
