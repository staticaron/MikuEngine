#pragma once

#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
	class TransformComponent;
}

namespace MikuEngine
{
	class MIKU_API TransformSystem
	{
	public:
		static void SerializeTransformComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeTransformComponent( TransformComponent& TransformComponent, const YAML::Node& node );

	private:
	};
}
