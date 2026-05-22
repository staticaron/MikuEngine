#include "Systems/CameraSystem.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

#include "Application.h"
#include "Components.h"
#include "Entity.h"
#include "Helpers/ImGuiHelper.h"

namespace MikuEngine
{
	glm::vec3 CameraSystem::GetWorldPosFromPixelPosition( const Entity& cameraEntity, const CameraComponent& cameraComponent, glm::vec2 pixelPosition )
	{
		float ndcX = ( pixelPosition.x / Application::GetDataContainer().GetGameResolution().x ) * 2 - 1;
		float ndcY = 1 - ( pixelPosition.y / Application::GetDataContainer().GetGameResolution().y ) * 2;

		glm::vec4 clipPos = { ndcX, ndcY, 0, 1 };

		glm::mat4 projViewMtxInv = glm::inverse( GetProjViewMatrix( cameraEntity, cameraComponent ) );

		return projViewMtxInv * clipPos;
	}

	glm::mat4 CameraSystem::GetMVPFromModelMatrix( const Entity& cameraEntity, const CameraComponent& cameraComponent, glm::mat4 modelMatrix )
	{
		return GetProjViewMatrix( cameraEntity, cameraComponent ) * modelMatrix;
	}

	glm::mat4 CameraSystem::GetProjViewMatrix( const Entity& cameraEntity, const CameraComponent& cameraComponent )
	{
		glm::mat4 projMatrix = GetProjMatrix( cameraComponent );
		glm::mat4 viewMatrix = GetViewMatrix( cameraEntity );

		return projMatrix * viewMatrix;
	}

	glm::mat4 CameraSystem::GetProjMatrix( const CameraComponent& cameraComponent )
	{
		auto camSize = cameraComponent.GetCameraSize();

		glm::mat4 projMatrix = glm::mat4( 1.0f );

		if ( cameraComponent.m_IsPerspective )
			projMatrix = glm::perspective( glm::radians( 60.0f ), Application::GetApplication()->GetDataContainer().GetGameAspectRatio(), 1.0f, 1000.0f );
		else
			projMatrix = glm::ortho( -camSize.x * 0.5f, camSize.x * 0.5f, -camSize.y * 0.5f, camSize.y * 0.5f, -1000.0f, 1000.0f );

		return projMatrix;
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
			ImGuiHelper::StartPropertyTable();

			ImGuiHelper::RenderTableItem( "Is Perspective", [ & ]() { ImGui::Checkbox( "##Is Perspective", &cameraComponent.m_IsPerspective ); } );
			ImGuiHelper::RenderTableItem( "Is Main Camera", [ & ]() { ImGui::Checkbox( "##Is Main Camera", &cameraComponent.m_IsMainCamera ); } );
			ImGuiHelper::RenderTableItem( "Zoom", [ & ]() { ImGui::DragFloat( "##Zoom", &cameraComponent.Zoom ); } );

			// Width has no meaning in perspective camera
			if ( cameraComponent.m_IsPerspective == false ) ImGui::DragInt( "Width", &cameraComponent.m_CameraWidth );

			ImGuiHelper::EndPropertyTable();
		}

		if ( !keep ) entity.RemoveComponent<CameraComponent>();
	}

	void CameraSystem::SerializeCameraComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto cameraComponent = entity.GetReadOnlyComponent<CameraComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "CameraComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "perspective" << YAML::Value << cameraComponent.IsPerspective();
		emitter << YAML::Key << "zoom" << YAML::Value << cameraComponent.GetZoom();
		emitter << YAML::Key << "maincamera" << YAML::Value << cameraComponent.IsMainCamera();
		emitter << YAML::Key << "width" << YAML::Value << cameraComponent.GetCameraSize().x;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void CameraSystem::DeSerializeCameraComponent( CameraComponent& cameraComponent, const YAML::Node& node )
	{
		bool perspective = node[ "perspective" ].as<bool>();
		float zoom = node[ "zoom" ].as<float>();
		bool isMainCamera = node[ "maincamera" ].as<bool>();
		int width = node[ "width" ].as<int>();

		cameraComponent.m_IsPerspective = perspective;
		cameraComponent.Zoom = zoom;
		cameraComponent.m_IsMainCamera = isMainCamera;
		cameraComponent.m_CameraWidth = width;
	}
}
