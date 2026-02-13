#include "Systems/CameraSystem.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

#include "Application.h"
#include "Components.h"
#include "Entity.h"
#include "Helpers/SerializationHelper.h"
#include "Logger.h"

namespace MikuEngine
{
	glm::mat4 CameraSystem::GetMVPFromModelMatrix( const Entity& cameraEntity, const CameraComponent& cameraComponent, glm::mat4 modelMatrix )
	{
		return GetProjViewMatrix( cameraEntity, cameraComponent ) * modelMatrix;
	}

	glm::mat4 CameraSystem::GetProjViewMatrix( const Entity& cameraEntity, const CameraComponent& cameraComponent )
	{
		auto camWidth = cameraComponent.GetCameraSize().x;
		auto camHeight = cameraComponent.GetCameraSize().y;

		glm::mat4 projMatrix = glm::ortho( -camWidth * 0.5f, camWidth * 0.5f, camHeight * 0.5f, -camHeight * 0.5f, -1000.0f, 1000.0f );
		glm::mat4 viewMatrix = GetViewMatrix( cameraEntity );

		return projMatrix * viewMatrix;
	}

	glm::mat4 CameraSystem::GetViewMatrix( const Entity& cameraEntity )
	{
		const auto& cameraTransform = cameraEntity.GetReadOnlyComponent<TransformComponent>();

		auto transform = glm::translate( glm::mat4( 1.0f ), cameraTransform.Position );

		auto rotationX = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		auto rotationY = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		auto rotationZ = glm::rotate( glm::mat4( 1.0f ), cameraTransform.Rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		auto rotation = rotationX * rotationY * rotationZ;

		return glm::inverse( transform * rotation );
	}

	void CameraSystem::CameraComponentRenderImGui( Entity entity, CameraComponent& cameraComponent )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "CameraComponent", &keep ) )
		{
			ImGui::Checkbox( "Is Main Camera", &cameraComponent.m_IsMainCamera );
			ImGui::DragFloat( "Zoom", &cameraComponent.Zoom );
			ImGui::DragInt( "Width", &cameraComponent.m_CameraWidth );
		}

		if ( !keep ) entity.RemoveComponent<CameraComponent>();
	}

	void CameraSystem::SerializeCameraComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto cameraComponent = entity.GetReadOnlyComponent<CameraComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "CameraComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "zoom" << YAML::Value << cameraComponent.GetZoom();
		emitter << YAML::Key << "isMainCamera" << YAML::Value << cameraComponent.IsMainCamera();
		emitter << YAML::Key << "width" << YAML::Value << cameraComponent.GetCameraSize().x;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void CameraSystem::DeSerializeCameraComponent( CameraComponent& cameraComponent, const YAML::Node& node )
	{
		float zoom = node[ "zoom" ].as<float>();
		bool isMainCamera = node[ "isMainCamera" ].as<bool>();
		int width = node[ "width" ].as<int>();

		cameraComponent.Zoom = zoom;
		cameraComponent.m_IsMainCamera = isMainCamera;
		cameraComponent.m_CameraWidth = width;
	}

}
