#pragma once

#include "yaml-cpp/emitter.h"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
	class ParticleEmitterComponent;
}

namespace MikuEngine
{
	class MIKU_API ParticleEmitterSystem
	{
	public:
		static void RenderParticles();

		static void ParticleEmitterComponentRenderImGui( Entity entity, ParticleEmitterComponent& particleSystemC );
		static void SerializeParticleEmitter( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeParticleEmitter( ParticleEmitterComponent& particleSystemC, const YAML::Node& node );
	};
}
