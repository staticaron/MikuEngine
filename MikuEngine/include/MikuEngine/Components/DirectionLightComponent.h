#pragma once

#include "glm/ext/vector_float3.hpp"

#include "Components/BaseComponent.h"
#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API DirectionalLightComponent : public BaseComponent
	{
		glm::vec3 Color;

		float Intensity;
		float AmbientIntensity;

		float SpecularStrength;
	};
}
