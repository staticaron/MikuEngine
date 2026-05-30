#include "Systems/ParticleSystem.h"

#include "imgui.h"
#include "yaml-cpp/yaml.h"

#include "Components/ParticleSystemComponent.h"
#include "Entity.h"
#include "Helpers/ImGuiHelper.h"
#include "Helpers/SerializationHelper.h"

namespace MikuEngine
{
	void ParticleSystem::RenderParticles() {}

	void ParticleSystem::ParticleSystemComponentRenderImGui( Entity entity, ParticleSystemComponent& particleSystemC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "ParitcleSystemComponent", &keep ) )
		{
			ImGuiHelper::StartPropertyTable();

			ImGuiHelper::RenderTableItem( "Max Particle Count", [ & ]() { ImGui::InputScalar( "##MaxParticleCount", ImGuiDataType_U32, &particleSystemC.MaxParticleCount ); } );
			ImGuiHelper::RenderTableItem( "Particle Size", [ & ]() { ImGui::DragFloat2( "##Particle Size", &particleSystemC.ParticleSize[ 0 ] ); } );
			ImGuiHelper::RenderTableItem( "Particle Spawn Radius", [ & ]() { ImGui::DragFloat( "##ParticleSizeRadius", &particleSystemC.SpawnRadius ); } );

			ImGuiHelper::EndPropertyTable();
		};

		if ( !keep ) entity.RemoveComponent<ParticleSystemComponent>();
	}

	void ParticleSystem::SerializeParticleSystem( const Entity& entity, YAML::Emitter& emitter )
	{
		auto particleSystem = entity.GetReadOnlyComponent<ParticleSystemComponent>();

		YAML::Node node;

		node[ "type" ] = "ParticleSystemComponent";

		YAML::Node properties;
		properties[ "max_particle" ] = particleSystem.MaxParticleCount;
		properties[ "particle_size" ] = YAML::Node( YAML::NodeType::Sequence );
		properties[ "particle_size" ].push_back( particleSystem.ParticleSize.x );
		properties[ "particle_size" ].push_back( particleSystem.ParticleSize.y );
		properties[ "spawn_radius" ] = particleSystem.SpawnRadius;

		node[ "values" ] = properties;

		emitter << node;
	}

	void ParticleSystem::DeSerializeParticleSystem( ParticleSystemComponent& particleSystemC, const YAML::Node& node )
	{
		unsigned int maxParticleCount = node[ "max_particle" ].as<unsigned int>();
		float spawnRadius = node[ "spawn_radius" ].as<float>();

		glm::vec2 particleSize;
		DecodeVec2( node[ "particle_size" ], particleSize );

		particleSystemC.ParticleSize = particleSize;
		particleSystemC.MaxParticleCount = maxParticleCount;
		particleSystemC.SpawnRadius = spawnRadius;
	}
}
