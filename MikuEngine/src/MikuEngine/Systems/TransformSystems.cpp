#include "Systems/TransformSystems.h"

#include "imgui.h"

#include "Components.h"
#include "Entity.h"
#include "Helpers/ImGuiHelper.h"
#include "Helpers/SerializationHelper.h"
#include "Logger.h"

namespace MikuEngine
{
	void TransformSystem::TransformComponentRenderImGui( Entity entity, TransformComponent& transformC )
	{
		if ( ImGui::CollapsingHeader( "TransformComponent" ) )
		{
			ImGuiHelper::StartPropertyTable();

			ImGuiHelper::RenderTableItem( "Position", [ & ]() { ImGui::DragFloat3( "##Position", &transformC.Position.x ); } );
			ImGuiHelper::RenderTableItem( "Rotation", [ & ]() { ImGui::DragFloat3( "##Rotation", &transformC.Rotation.x ); } );
			ImGuiHelper::RenderTableItem( "Scale", [ & ]() { ImGui::DragFloat3( "##Scale", &transformC.Scale.x ); } );

			ImGuiHelper::EndPropertyTable();
		}
	}

	void TransformSystem::SerializeTransformComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;
		auto transform = entity.GetReadOnlyComponent<TransformComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "TransformComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << transform.Position.x << transform.Position.y << transform.Position.z << YAML::EndSeq;
		emitter << YAML::Key << "rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq << transform.Rotation.x << transform.Rotation.y << transform.Rotation.z << YAML::EndSeq;
		emitter << YAML::Key << "scale" << YAML::Value << YAML::Flow << YAML::BeginSeq << transform.Scale.x << transform.Scale.y << transform.Scale.z << YAML::EndSeq;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void TransformSystem::DeSerializeTransformComponent( TransformComponent& transformC, const YAML::Node& node )
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		DecodeVec3( node[ "position" ], position );
		DecodeVec3( node[ "rotation" ], rotation );
		DecodeVec3( node[ "scale" ], scale );

		transformC.Position = position;
		transformC.Rotation = rotation;
		transformC.Scale = scale;
	}

	glm::mat4 TransformSystem::GetTransformMatrix( const Scene& scene, const entt::entity& entt )
	{
		const auto& transformC = scene.GetRegistry().get<TransformComponent>( entt );
		const auto& dataC = scene.GetRegistry().get<DataComponent>( entt );

		if ( dataC.ParentUUID.has_value() )
		{
			const auto& parentEntt = scene.GetEntityByID( dataC.ParentUUID.value() );
			MIKU_ASSERT( parentEntt.has_value(), "This Entity has a parentUUID but that parentEntity is not loaded in the scene!" );
			return GetTransformMatrix( scene, parentEntt.value() ) * transformC.GetModelMatrix();
		}
		else
			return transformC.GetModelMatrix();
	}
}
