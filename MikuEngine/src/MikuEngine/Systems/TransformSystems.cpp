#include "Systems/TransformSystems.h"

#include "Components.h"
#include "Entity.h"
#include "Helpers/SerializationHelper.h"

namespace MikuEngine
{
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
}
