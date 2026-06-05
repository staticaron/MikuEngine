#pragma once

#include "Core.h"
#include "ParticleEmitter.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API ParticleEmitterManager
	{
	public:
		void LoadParticleEmitter( const UUID& entityUUID, const ParticleEmitterProperties& emitterProperties );

	private:
		std::unordered_map<UUID, ParticleEmitter> m_ParticleEmitters;
	};
}
