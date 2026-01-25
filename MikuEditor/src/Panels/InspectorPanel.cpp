#include "Panels/InspectorPanel.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Logger.h"
#include "MikuEngine/Scene/Scene.h"
#include "MikuEngine/Systems/CameraSystem.h"

#include "Layers/EditorLayer.h"
#include "Panels/Panels.h"

namespace MikuEditor
{
	void InspectorPanel::RenderInspectorPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Inspector" );

		auto selectedEntity = scene.GetSelectedEntity();

		if ( selectedEntity.has_value() )
		{
			if ( selectedEntity.value().HasComponent<MikuEngine::DataComponent>() )
			{
				auto& dataC = selectedEntity.value().GetComponent<MikuEngine::DataComponent>();

				ImGui::TextUnformatted( "Entity" );
				ImGui::SameLine();

				char entityName[ 255 ];

				std::copy( dataC.EntityName.begin(), dataC.EntityName.begin() + dataC.EntityName.length(), entityName );
				entityName[ dataC.EntityName.length() ] = '\0';

				ImGui::InputText( "##DataComponent", entityName, 255 );

				if ( ImGui::IsItemDeactivatedAfterEdit() ) dataC.EntityName = std::string( entityName );
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::TransformComponent>() )
			{
				auto& transformC = selectedEntity.value().GetComponent<MikuEngine::TransformComponent>();

				if ( ImGui::TreeNode( "TransformComponent" ) )
				{
					ImGui::DragFloat3( "Position", &transformC.Position.x );
					ImGui::DragFloat3( "Rotation", &transformC.Rotation.x );
					ImGui::DragFloat3( "Scale", &transformC.Scale.x );

					ImGui::TreePop();
				}
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::SpriteRendererComponent>() )
			{
				auto& spriteRendererC = selectedEntity.value().GetComponent<MikuEngine::SpriteRendererComponent>();

				if ( ImGui::TreeNode( "SpriteRendererComponent" ) )
				{
					auto textureUUID = spriteRendererC.TextureIdentifier;

					auto texture = appLevelStuff.GetTextureManager().GetTexture( textureUUID.value() );
					auto textureName = appLevelStuff.GetTextureManager().GetTextureName( textureUUID.value() );

					DISABLED_IMGUI( ImGui::Button( textureName.c_str() ) );
					ImGui::SameLine();
					if ( ImGui::Button( "EDIT..." ) )
					{
						editorLayer.m_TextureSelectionWindow.emplace_back( selectedEntity.value().GetUUID() );
						MIKU_CLIENT_INFO( "Entity for which texture selection window was opened {}", std::to_string( selectedEntity.value().GetUUID() ) );
					}

					ImGui::DragFloat4( "Tint", &spriteRendererC.Tint.x );

					ImGui::TreePop();
				};
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::CameraComponent>() )
			{
				auto& cameraC = selectedEntity.value().GetComponent<MikuEngine::CameraComponent>();
				MikuEngine::CameraSystem::CameraComponentRenderImGui( cameraC );
			}
		}

		// Show Add Component Button ( ACTIVE if scene has selected entity otherwise DISABLED)
		if ( scene.GetSelectedEntity().has_value() )
		{
			if ( MikuEngine::ImguiManager::FullWidthButton( "Add Component" ) ) scene.GetSelectedEntity()->AddComponent<MikuEngine::SpriteRendererComponent>();
		}
		else
		{
			DISABLED_IMGUI( MikuEngine::ImguiManager::FullWidthButton( "Add Component" ) );
		}

		if ( selectedEntity.has_value() )
		{
			auto& idC = selectedEntity.value().GetComponent<MikuEngine::IDComponent>();
			std::string id = "ID : " + std::to_string( idC.ID );

			ImGui::TextUnformatted( id.c_str() );
		}

		ImGui::End();
	}
}
