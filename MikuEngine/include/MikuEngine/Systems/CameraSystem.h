#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "yaml-cpp/emitter.h"

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
