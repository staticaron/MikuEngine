#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
	class CameraComponent;
}

namespace MikuEngine
{
	class MIKU_API CameraSystem
	{
	public:
		static glm::vec3 GetWorldPosFromPixelPosition( const Entity& cameraEntity, const CameraComponent& cameraComponent, glm::vec2 pixelPosition );

		static glm::mat4 GetMVPFromModelMatrix( const Entity& entity, const CameraComponent& cameraComponent, glm::mat4 modelMatrix );
		static glm::mat4 GetProjViewMatrix( const Entity& entity, const CameraComponent& cameraComponent );
		static glm::mat4 GetViewMatrix( const Entity& entity );
		static glm::mat4 GetProjMatrix( const CameraComponent& cameraC );

		static void CameraComponentRenderImGui( Entity entity, CameraComponent& cameraComponent );
		static void SerializeCameraComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeCameraComponent( CameraComponent& cameraComponent, const YAML::Node& emitter );
	};
}
