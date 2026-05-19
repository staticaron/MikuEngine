#pragma once

#include "entt/entt.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
	class Scene;
	class TransformComponent;
}

namespace MikuEngine
{
	class MIKU_API TransformSystem
	{
	public:
		static void TransformComponentRenderImGui( Entity entity, TransformComponent& transformC );
		static void SerializeTransformComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeTransformComponent( TransformComponent& transformComponent, const YAML::Node& node );

		static glm::mat4 GetTransformMatrix( const Scene& scene, const entt::entity& entt );

	private:
	};
}
