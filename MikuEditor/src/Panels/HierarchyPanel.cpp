#include "Panels/HierarchyPanel.h"

#include "Entity.h"
#include "MikuEngine/Managers/ImguiManager.h"
#include "UUID.h"

namespace MikuEditor
{
	void HierarchyPanel::RenderNode( MikuEngine::Scene& scene, std::unordered_map<MikuEngine::UUID, std::vector<MikuEngine::UUID>> parentChildren, MikuEngine::UUID currentUUID )
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

		auto entityUUID = currentUUID;
		auto entity = scene.GetEntityByID( currentUUID );
		auto selectedItem = scene.GetSelectedItem();

		if ( selectedItem.has_value() && selectedItem.value().type == MikuEngine::SelectableType::ENTITY && selectedItem.value().uuid == entityUUID )
		{
			treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		auto children = parentChildren[ currentUUID ];

		if ( children.size() == 0 ) treeNodeFlags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_Leaf;

		bool nodeOpen = ImGui::TreeNodeEx( entity.value().GetNamedIdentifier().c_str(), treeNodeFlags );

		if ( ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() )
		{
			scene.SetSelectedItem( entityUUID, MikuEngine::SelectableType::ENTITY );
		}

		if ( ImGui::BeginDragDropSource() )
		{
			ImGui::SetDragDropPayload( "ENTITY_DRAG_DROP_PAYLOAD", &entityUUID, sizeof( uint64_t ) );
			ImGui::SetTooltip( "%s", entity->GetNamedIdentifier().c_str() );
			ImGui::EndDragDropSource();
		}

		if ( ImGui::BeginDragDropTarget() )
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "ENTITY_DRAG_DROP_PAYLOAD" );
			if ( payload != nullptr )
			{
				const MikuEngine::UUID uuid = *( MikuEngine::UUID* )( payload->Data );
				if ( uuid != entityUUID )
				{
					auto draggedEntity = scene.GetEntityByID( uuid );
					if ( draggedEntity.has_value() == true ) draggedEntity.value().SetParent( entityUUID );
				};
			}

			ImGui::EndDragDropTarget();
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
		std::vector<MikuEngine::UUID> rootNodes;

		entityList.reserve( entities.size() );

		float buttonSizeToReserve = ImGui::GetFrameHeightWithSpacing();
		ImVec2 childWindowSize = { 0, -buttonSizeToReserve };

		if ( ImGui::BeginChild( "##Entities", childWindowSize ) )
		{
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
					rootNodes.push_back( entity.GetUUID() );
				}
			}

			for ( auto rootNode : rootNodes )
			{
				RenderNode( scene, entityList, rootNode );
			}

			ImGui::EndChild();
		}

		if ( ImGui::BeginDragDropTarget() )
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "ENTITY_DRAG_DROP_PAYLOAD" );
			if ( payload != nullptr )
			{
				const MikuEngine::UUID uuid = *( MikuEngine::UUID* )( payload->Data );
				auto draggedEntity = scene.GetEntityByID( uuid );
				if ( draggedEntity.has_value() == true ) draggedEntity.value().SetParent( std::nullopt );
			}

			ImGui::EndDragDropTarget();
		}

		if ( ImGui::IsItemClicked( ImGuiMouseButton_Left ) ) scene.RemoveSelectedItem();

		if ( MikuEngine::ImguiManager::FullWidthButton( "ADD" ) ) scene.CreateEntity( "New GameObject", &scene, std::nullopt );

		ImGui::End();
	}
}
