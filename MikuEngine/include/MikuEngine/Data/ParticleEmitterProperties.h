#pragma once

#include "glm/ext/vector_float2.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API ParticleEmitterProperties
	{
		unsigned int MaxParticleCount = 100;
		glm::vec2 ParticleSize = { 0.2f, 0.2f };
		float ParticleSpawnRadius = 1.0f;
	};
}
