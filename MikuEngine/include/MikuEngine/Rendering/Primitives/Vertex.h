#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API Vertex
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 UV = { 0.0f, 0.0f };
	};

	struct MIKU_API Vertex3D
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Normal = { 0.0f, 0.0f, 0.0f };
		glm::vec2 UV = { 0.0f, 0.0f };
	};
}
