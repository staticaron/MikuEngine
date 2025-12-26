#include "Scene/SceneUIElements.h"

#include "Entity.h"
#include "Managers/ImguiManager.h"

namespace MikuEngine
{
	void SceneUIElements::RenderMenuBar( Scene& scene )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "Scene" ) )
			{
				ImGui::Separator();
				if ( ImGui::MenuItem( "Save...", "CTRL+S" ) ) scene.Save( "main.miku" );
				ImGui::Separator();
				if ( ImGui::MenuItem( "Open...", "CTRL+O" ) ) scene.Load( "main.miku" );
				ImGui::Separator();
				if ( ImGui::MenuItem( "Clean", "CTRL+W" ) ) scene.Clean();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void SceneUIElements::RenderHierarchy( Scene& scene )
	{
		ImGui::Begin( "Hierarchy" );

		float footer_height_to_reserve = ImGui::GetFrameHeightWithSpacing();

		if ( ImGui::BeginChild( "##Entities", ImVec2( 0, -footer_height_to_reserve ), false ) )
		{
			if ( ImGui::BeginListBox( "##Scene Hierarchy", ImVec2( -FLT_MIN, -FLT_MIN ) ) )
			{
				int item_highlighted_idx = -1;

				auto entities = scene.GetAllEntities();
				std::vector<std::string> entitiyNames;

				for ( int x = 0; x < entities.size(); x++ )
				{
					auto dc = entities[ x ].GetComponent<DataComponent>();
					auto ic = entities[ x ].GetComponent<IDComponent>();

					bool is_selected = ( scene.m_SelectedEntityID == ic.ID );
					ImGuiSelectableFlags flags = ( item_highlighted_idx == x ) ? ImGuiSelectableFlags_Highlight : 0;

					if ( ImGui::Selectable( entities[ x ].GetNamedIdentifier().c_str(), is_selected, flags ) ) scene.m_SelectedEntityID = ic.ID;

					if ( is_selected ) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndListBox();
			}

			ImGui::EndChild();
		}

		if ( ImguiManager::FullWidthButton( "ADD" ) ) scene.CreateEntity( "New GameObject", &scene );

		ImGui::End();
	}

	void SceneUIElements::RenderInspector( Scene& scene )
	{
		ImGui::Begin( "Inspector" );

		auto selectedEntity = scene.GetSelectedEntity();

		if ( selectedEntity.has_value() )
		{
			if ( selectedEntity.value().HasComponent<DataComponent>() )
			{
				auto& dataC = selectedEntity.value().GetComponent<DataComponent>();

				ImGui::TextUnformatted( "Entity" );
				ImGui::SameLine();

				char entityName[ 255 ];

				std::copy( dataC.EntityName.begin(), dataC.EntityName.begin() + dataC.EntityName.length(), entityName );
				entityName[ dataC.EntityName.length() ] = '\0';

				ImGui::InputText( "##DataComponent", entityName, 255 );

				if ( ImGui::IsItemDeactivatedAfterEdit() ) dataC.EntityName = std::string( entityName );
			}

			if ( selectedEntity.value().HasComponent<TransformComponent>() )
			{
				auto& transformC = selectedEntity.value().GetComponent<TransformComponent>();

				if ( ImGui::TreeNode( "TransformComponent" ) )
				{
					ImGui::DragFloat3( "Position", &transformC.Position.x );
					ImGui::DragFloat3( "Rotation", &transformC.Rotation.x );
					ImGui::DragFloat3( "Scale", &transformC.Scale.x );

					ImGui::TreePop();
				}
			}

			if ( selectedEntity.value().HasComponent<SpriteRendererComponent>() )
			{
				auto& spriteRendererC = selectedEntity.value().GetComponent<SpriteRendererComponent>();

				if ( ImGui::TreeNode( "SpriteRendererComponent" ) )
				{
					char textureBuffer[ 255 ];
					std::copy( spriteRendererC.TextureIdentifier.begin(), spriteRendererC.TextureIdentifier.begin() + spriteRendererC.TextureIdentifier.length(), textureBuffer );
					textureBuffer[ spriteRendererC.TextureIdentifier.length() ] = '\0';

					ImGui::InputText( "Texture", textureBuffer, 255 );
					if ( ImGui::IsItemDeactivatedAfterEdit() ) spriteRendererC.TextureIdentifier = std::string( textureBuffer );

					ImGui::DragFloat4( "Tint", &spriteRendererC.Tint.x );

					ImGui::TreePop();
				}
			}

			if ( selectedEntity.value().HasComponent<CameraComponent>() )
			{
				auto& cameraC = selectedEntity.value().GetComponent<CameraComponent>();

				if ( ImGui::TreeNode( "CameraComponent" ) )
				{
					ImGui::DragFloat( "Zoom", &cameraC.Zoom );

					ImGui::TreePop();
				}
			}
		}

		if ( ImguiManager::FullWidthButton( "Add Component" ) )
		{
			scene.GetSelectedEntity()->AddComponent<SpriteRendererComponent>();
		}
		ImGui::End();
	}

	void SceneUIElements::RenderProjectView( Scene& scene ) {}
}
