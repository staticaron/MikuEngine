#include "Managers/ParticleEmitterManager.h"

namespace MikuEngine
{
	void ParticleEmitterManager::LoadParticleEmitter( const UUID& entityUUID, const ParticleEmitterProperties& emitterProperties )
	{
		m_ParticleEmitters[ entityUUID ].Init( emitterProperties );
	}
};
