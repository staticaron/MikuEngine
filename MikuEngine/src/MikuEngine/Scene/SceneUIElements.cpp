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

		int item_highlighted_idx = -1;

		if ( ImGui::BeginListBox( "##Scene Hierarchy", ImVec2( -FLT_MIN, -FLT_MIN ) ) )
		{
			auto entities = scene.GetAllEntities();
			std::vector<std::string> entitiyNames;

			for ( int x = 0; x < entities.size(); x++ )
			{
				auto dc = entities[ x ].GetComponent<DataComponent>();
				auto ic = entities[ x ].GetComponent<IDComponent>();

				bool is_selected = ( scene.m_SelectedEntityID == ic.ID );
				ImGuiSelectableFlags flags = ( item_highlighted_idx == x ) ? ImGuiSelectableFlags_Highlight : 0;

				if ( ImGui::Selectable( dc.EntityName.c_str(), is_selected, flags ) ) scene.m_SelectedEntityID = ic.ID;

				if ( is_selected ) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndListBox();
		}

		ImGui::End();
	}

	void SceneUIElements::RenderInspector( Scene& scene )
	{
		ImGui::Begin( "Inspector" );

		auto selectedEntity = scene.GetSelectedEntity();

		if ( selectedEntity.has_value() )
		{
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

		ImGui::End();
	}

	void SceneUIElements::RenderProjectView( Scene& scene ) {}
}
