#pragma once

#include <functional>

#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class Entity;
	class AppLevelStuff;
	class SkyboxComponent;
	class CameraData;
}

namespace MikuEngine
{
	class MIKU_API SkyboxRendererSystem
	{
	public:
		static void RenderSkybox( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData );

		static void SkyboxComponentRenderImGui( Entity entity, SkyboxComponent& spriteRendererC, std::function<void()> modelEditBtnCallback, std::function<void()> materialEditBtnCallback );
		static void SerializeSkyboxComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeSkyboxComponent( SkyboxComponent& spriteRendererC, const YAML::Node& node );
	};
}
