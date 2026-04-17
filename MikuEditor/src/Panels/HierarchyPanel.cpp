#include "Panels/HierarchyPanel.h"

#include "Components.h"
#include "MikuEngine/Managers/ImguiManager.h"

namespace MikuEditor
{
	void HierarchyPanel::RenderHierarchy( MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Hierarchy" );

		float footer_height_to_reserve = ImGui::GetFrameHeightWithSpacing();

		ImGui::PushStyleColor( ImGuiCol_FrameBg, { 0.0f, 0.0f, 0.0f, 1.0f } );

		if ( ImGui::BeginChild( "##Entities", ImVec2( 0, -footer_height_to_reserve ), false ) )
		{
			if ( ImGui::BeginListBox( "##Scene Hierarchy", ImVec2( -FLT_MIN, -FLT_MIN ) ) )
			{
				int item_highlighted_idx = -1;

				auto entities = scene.GetAllEntities();
				std::vector<std::string> entitiyNames;

				for ( int x = 0; x < entities.size(); x++ )
				{
					auto dc = entities[ x ].GetComponent<MikuEngine::DataComponent>();
					auto ic = entities[ x ].GetComponent<MikuEngine::IDComponent>();

					bool is_selected = ( scene.GetSelectedItem().has_value() ? scene.GetSelectedItem().value().uuid == ic.ID : false );
					ImGuiSelectableFlags flags = ( item_highlighted_idx == x ) ? ImGuiSelectableFlags_Highlight : 0;

					if ( ImGui::Selectable( entities[ x ].GetNamedIdentifier().c_str(), is_selected, flags ) ) scene.SetSelectedItem( ic.ID, MikuEngine::SelectableType::ENTITY );

					if ( is_selected ) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndListBox();
			}

			ImGui::EndChild();

			ImGui::PopStyleColor();
		}

		if ( MikuEngine::ImguiManager::FullWidthButton( "ADD" ) ) scene.CreateEntity( "New GameObject", &scene );

		ImGui::End();
	}
}
