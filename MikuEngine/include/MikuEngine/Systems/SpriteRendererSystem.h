#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/emitter.h"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class Entity;
	class AppLevelStuff;
	class SpriteRendererComponent;
	class CameraData;
}

namespace MikuEngine
{
	class MIKU_API SpriteRendererSystem
	{
	public:
		static void RenderSprite( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData );

		static void SpriteRendererComponentRenderImGui( Entity entity, SpriteRendererComponent& spriteRendererC, std::function<void()> textureEditBtnCallback, std::function<void()> shaderEditBtnCallback );
		static void SerializeSpriteRendererComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeSpriteRendererComponent( SpriteRendererComponent& spriteRendererC, const YAML::Node& node );
	};
}
