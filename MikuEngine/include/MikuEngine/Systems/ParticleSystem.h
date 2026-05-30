#pragma once

#include "yaml-cpp/emitter.h"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
	class ParticleSystemComponent;
}

namespace MikuEngine
{
	class MIKU_API ParticleSystem
	{
	public:
		static void RenderParticles();

		static void ParticleSystemComponentRenderImGui( Entity entity, ParticleSystemComponent& particleSystemC );
		static void SerializeParticleSystem( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeParticleSystem( ParticleSystemComponent& particleSystemC, const YAML::Node& node );
	};
}
