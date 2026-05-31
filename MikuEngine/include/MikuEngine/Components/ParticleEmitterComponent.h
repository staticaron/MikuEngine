#pragma once

#include "glm/ext/vector_float2.hpp"

#include "BaseComponent.h"
#include "Core.h"

namespace MikuEngine
{
	class MIKU_API ParticleEmitterComponent : public BaseComponent
	{
	public:
		unsigned int MaxParticleCount = 1000;
		glm::vec2 ParticleSize = { 0.1f, 0.1f };
		float SpawnRadius = 1.0f;
	};
}
