#include "Panels/InspectorPanel.h"

#include "Entity.h"

#include "Panels/Panels.h"

namespace MikuEditor
{
	void InspectorPanel::RenderInspectorPanel( EditorLayer& layer, MikuEngine::Scene& scene )
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
					DISABLED_IMGUI( ImGui::Button( spriteRendererC.TextureIdentifier.c_str() ) );
					ImGui::SameLine();
					if ( ImGui::Button( "EDIT..." ) )
					{
					}

					ImGui::DragFloat4( "Tint", &spriteRendererC.Tint.x );

					ImGui::TreePop();
				};
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::CameraComponent>() )
			{
				auto& cameraC = selectedEntity.value().GetComponent<MikuEngine::CameraComponent>();

				if ( ImGui::TreeNode( "CameraComponent" ) )
				{
					ImGui::DragFloat( "Zoom", &cameraC.Zoom );

					ImGui::TreePop();
				}
			}
		}

		if ( MikuEngine::ImguiManager::FullWidthButton( "Add Component" ) )
		{
			scene.GetSelectedEntity()->AddComponent<MikuEngine::SpriteRendererComponent>();
		}
		ImGui::End();
	}
}
