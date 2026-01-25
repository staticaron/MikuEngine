#include "Systems/CameraSystem.h"

#include "Components/CameraComponent.h"
#include "Entity.h"
#include "Helpers/SerializationHelper.h"
#include "Logger.h"
#include "Managers/ImguiManager.h"

namespace MikuEngine
{
	glm::mat4 CameraSystem::GetMVPFromModelMatrix( const Entity& cameraEntity, const CameraComponent& cameraComponent, glm::mat4 modelMatrix )
	{
		return GetProjViewMatrix( cameraEntity, cameraComponent ) * modelMatrix;
	}

	glm::mat4 CameraSystem::GetProjViewMatrix( const Entity& cameraEntity, const CameraComponent& cameraComponent )
	{
		glm::mat4 projMatrix = glm::ortho( 0.0f, cameraComponent.m_Resolution.x, cameraComponent.m_Resolution.y, 0.0f, -1000.0f, 1000.0f );
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

	void CameraSystem::CameraComponentRenderImGui( CameraComponent& cameraComponent )
	{
		if ( ImGui::TreeNode( "CameraComponent" ) )
		{
			ImGui::Checkbox( "Is Main Camera", &cameraComponent.m_IsMainCamera );
			ImGui::DragFloat( "Zoom", &cameraComponent.Zoom );
			ImGui::DragFloat2( "Resolution", &cameraComponent.m_Resolution[ 0 ] );

			ImGui::TreePop();
		}
	}

	void CameraSystem::SerializeCameraComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto cameraComponent = entity.GetReadOnlyComponent<CameraComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "CameraComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "zoom" << YAML::Value << cameraComponent.Zoom;
		emitter << YAML::Key << "isMainCamera" << YAML::Value << cameraComponent.m_IsMainCamera;
		emitter << YAML::Key << "resolution" << YAML::Value << YAML::Flow << YAML::BeginSeq << cameraComponent.m_Resolution.x << cameraComponent.m_Resolution.y << YAML::EndSeq;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void CameraSystem::DeSerializeCameraComponent( CameraComponent& cameraComponent, const YAML::Node& node )
	{
		float zoom = node[ "zoom" ].as<float>();
		bool isMainCamera = node[ "isMainCamera" ].as<bool>();

		glm::vec2 resolution;
		DecodeVec2( node[ "resolution" ], resolution );

		MIKU_CORE_INFO( "Resolution: {} {}", resolution.x, resolution.y );

		cameraComponent.Zoom = zoom;
		cameraComponent.m_IsMainCamera = isMainCamera;
		cameraComponent.m_Resolution = resolution;
	}

}
