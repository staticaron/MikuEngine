#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class Entity;
	class AppLevelStuff;
	class SpriteRendererComponent;
}

namespace MikuEngine
{
	struct CameraData
	{
		glm::mat4 viewMatrix;
		glm::vec2 cameraResolution;
	};

	class MIKU_API RenderingSystem
	{
	public:
		static void RenderSprite( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData );
		static void ClearColor( glm::vec4 color );

		static void SpriteRendererComponentRenderImGui( SpriteRendererComponent& spriteRendererC, std::function<void()> textureEditBtnCallback );
		static void SerializeSpriteRendererComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeSpriteRendererComponent( SpriteRendererComponent& spriteRendererC, const YAML::Node& node );
	};
}
