#include "Systems/LightingSystem.h"

#include "imgui.h"

#include "Components/DirectionLightComponent.h"
#include "Entity.h"
#include "Helpers/ImGuiHelper.h"
#include "Helpers/SerializationHelper.h"

namespace MikuEngine
{
	void LightingSystem::DirectionalLightComponentRenderImGui( Entity entity, DirectionalLightComponent& directionalLightC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "DirectionalLightComponent", &keep ) )
		{
			ImGuiHelper::StartPropertyTable();

			ImGuiHelper::RenderTableItem( "Color", [ & ]() { ImGui::ColorEdit3( "##Color", &directionalLightC.Color[ 0 ] ); } );

			ImGuiHelper::RenderTableItem( "Light Strength", [ & ]() { ImGui::DragFloat( "##Light Strength", &directionalLightC.Intensity ); } );
			ImGuiHelper::RenderTableItem( "Ambient Intensity", [ & ]() { ImGui::DragFloat( "##Ambient Intensity", &directionalLightC.AmbientIntensity ); } );

			ImGuiHelper::RenderTableItem( "Specular Strength", [ & ]() { ImGui::DragFloat( "##Specular Strength", &directionalLightC.SpecularStrength ); } );

			ImGuiHelper::EndPropertyTable();
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
		emitter << YAML::Key << "ambient_intensity" << YAML::Value << directionalLight.AmbientIntensity;
		emitter << YAML::Key << "specular_strength" << YAML::Value << directionalLight.SpecularStrength;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void LightingSystem::DeSerializeDirectionalLightComponent( DirectionalLightComponent& directionalLightC, const YAML::Node& node )
	{
		glm::vec3 color;
		DecodeVec3( node[ "color" ], color );

		directionalLightC.Color = color;

		directionalLightC.Intensity = node[ "intensity" ].as<float>();
		directionalLightC.AmbientIntensity = node[ "ambient_intensity" ].as<float>();
		directionalLightC.SpecularStrength = node[ "specular_strength" ].as<float>();
	}
}
