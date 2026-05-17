#include "Panels/HierarchyPanel.h"

#include "Entity.h"
#include "MikuEngine/Managers/ImguiManager.h"
#include "UUID.h"

namespace MikuEditor
{
	void HierarchyPanel::RenderNode( MikuEngine::Scene& scene, const MikuEngine::Entity& entity )
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

		auto entityUUID = entity.GetUUID();
		auto selectedItem = scene.GetSelectedItem();
		if ( selectedItem.has_value() && selectedItem.value().type == MikuEngine::SelectableType::ENTITY && selectedItem.value().uuid == entityUUID )
		{
			treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		auto treeNodeName = ( entity.GetParent().has_value() ? entity.GetParent()->ToString() : "no parent" ) + " - " + entity.GetNamedIdentifier();

		bool nodeOpen = ImGui::TreeNodeEx( treeNodeName.c_str(), treeNodeFlags );

		if ( ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() )
		{
			scene.SetSelectedItem( entityUUID, MikuEngine::SelectableType::ENTITY );
		}

		if ( nodeOpen )
		{
			// Render Child Nodes
			ImGui::TreePop();
		}
	}

	void HierarchyPanel::RenderHierarchy( MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Hierarchy" );

		auto entities = scene.GetAllEntities();

		for ( int x = 0; x < entities.size(); x++ )
		{
			RenderNode( scene, entities[ x ] );
		}

		if ( MikuEngine::ImguiManager::FullWidthButton( "ADD" ) ) scene.CreateEntity( "New GameObject", &scene, std::nullopt );

		ImGui::End();
	}
}
