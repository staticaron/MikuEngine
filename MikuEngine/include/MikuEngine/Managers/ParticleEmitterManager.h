#pragma once

#include "Core.h"

namespace MikuEngine
{
	struct ParticleEmitterProperties;
}

namespace MikuEngine
{
	class MIKU_API ParticleEmitterManager
	{
	public:
		void LoadParticleEmitter( const ParticleEmitterProperties& emitterProperties );

	private:
	};
}
