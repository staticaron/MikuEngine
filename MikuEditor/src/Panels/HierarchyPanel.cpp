#include "Panels/HierarchyPanel.h"

#include "Entity.h"
#include "MikuEngine/Managers/ImguiManager.h"
#include "UUID.h"

namespace MikuEditor
{
	void HierarchyPanel::RenderNode( MikuEngine::Scene& scene, std::unordered_map<MikuEngine::UUID, std::vector<MikuEngine::UUID>> parentChildren, MikuEngine::UUID currentUUID )
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

		auto entityUUID = currentUUID;
		auto entity = scene.GetEntityByID( currentUUID );
		auto selectedItem = scene.GetSelectedItem();

		if ( selectedItem.has_value() && selectedItem.value().type == MikuEngine::SelectableType::ENTITY && selectedItem.value().uuid == entityUUID )
		{
			treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		auto children = parentChildren[ currentUUID ];

		if ( children.size() == 0 ) treeNodeFlags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf;

		bool nodeOpen = ImGui::TreeNodeEx( entity.value().GetNamedIdentifier().c_str(), treeNodeFlags );

		if ( ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() )
		{
			scene.SetSelectedItem( entityUUID, MikuEngine::SelectableType::ENTITY );
		}

		if ( nodeOpen )
		{
			for ( auto child : children )
			{
				RenderNode( scene, parentChildren, child );
			}

			ImGui::TreePop();
		}
	}

	void HierarchyPanel::RenderHierarchy( MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Hierarchy" );

		auto entities = scene.GetAllEntities();

		std::unordered_map<MikuEngine::UUID, std::vector<MikuEngine::UUID>> entityList;
		entityList.reserve( entities.size() );

		for ( int x = 0; x < entities.size(); x++ )
		{
			auto& entity = entities[ x ];
			auto entityParent = entity.GetParent();

			if ( entityParent.has_value() )
			{
				entityList[ entityParent.value() ].push_back( entity.GetUUID() );
			}
			else
			{
				entityList[ entity.GetUUID() ] = {};
			}
		}

		for ( auto& [ parentUUID, parentChildren ] : entityList )
		{
			RenderNode( scene, entityList, parentUUID );
		}

		if ( MikuEngine::ImguiManager::FullWidthButton( "ADD" ) ) scene.CreateEntity( "New GameObject", &scene, std::nullopt );

		ImGui::End();
	}
}
