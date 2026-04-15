#include "Systems/LightingSystem.h"

#include "imgui.h"

#include "Components/DirectionLightComponent.h"
#include "Entity.h"
#include "Helpers/SerializationHelper.h"

namespace MikuEngine
{
	void LightingSystem::DirectionalLightComponentRenderImGui( Entity entity, DirectionalLightComponent& directionalLightC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "DirectionalLightComponent", &keep ) )
		{
			ImGui::ColorEdit3( "Color", &directionalLightC.Color[ 0 ] );
			ImGui::DragFloat( "Intensity", &directionalLightC.Intensity );
		}

		if ( !keep ) entity.RemoveComponent<DirectionalLightComponent>();
	}

	void LightingSystem::SerializeDirectionalLightComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto directionalLight = entity.GetReadOnlyComponent<DirectionalLightComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "DirectionalLightComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "color" << YAML::Value << YAML::Flow << YAML::BeginSeq << directionalLight.Color.x << directionalLight.Color.y << directionalLight.Color.z << YAML::EndSeq;
		emitter << YAML::Key << "intensity" << YAML::Value << directionalLight.Intensity;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void LightingSystem::DeSerializeDirectionalLightComponent( DirectionalLightComponent& directionalLightC, const YAML::Node& node )
	{
		glm::vec3 color;
		DecodeVec3( node[ "color" ], color );

		directionalLightC.Color = color;
		directionalLightC.Intensity = node[ "intensity" ].as<float>();
	}
}
