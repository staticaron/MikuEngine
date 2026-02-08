#pragma once

#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class Entity;
	class NativeScriptComponent;
}

namespace MikuEngine
{
	struct MIKU_API ScriptExecutionSystem
	{
		static void ExecuteScripts( Scene& scene );

		static void NativeScriptComponentRenderImGui( NativeScriptComponent& nativeScriptComponent );
		static void SerializeNativeScriptComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeNativeScriptComponent( NativeScriptComponent& nativeScriptC, const YAML::Node& node );
	};
}
