#pragma once

#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/node.h"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class Entity;
	class DirectionalLightComponent;
}

namespace MikuEngine
{
	class MIKU_API LightingSystem
	{
	public:
		static void DirectionalLightComponentRenderImGui( Entity entity, DirectionalLightComponent& directionalLightC );
		static void SerializeDirectionalLightComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeDirectionalLightComponent( DirectionalLightComponent& directionalLightC, const YAML::Node& node );
	};
}
