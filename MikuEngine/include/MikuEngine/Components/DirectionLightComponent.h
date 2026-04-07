#pragma once

#include "glm/ext/vector_float3.hpp"

#include "Components/BaseComponent.h"
#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API DirectionalLightComponent : BaseComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		float Intensity;
	};
}
