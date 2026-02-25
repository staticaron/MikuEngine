#include "Panels/InspectorPanel.h"

#include "Components.h"
#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Data/SelectableItem.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Scene/Scene.h"
#include "MikuEngine/Systems/CameraSystem.h"
#include "MikuEngine/Systems/ScriptExecutionSystem.h"
#include "MikuEngine/Systems/SpriteRendererSystem.h"
#include "MikuEngine/Systems/TransformSystems.h"

#include "imgui.h"
#include "Layers/EditorLayer.h"
#include "Panels/Panels.h"

namespace MikuEditor
{
	void InspectorPanel::RenderInspectorPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Inspector" );

		auto selectedItem = scene.GetSelectedItem();

		if ( selectedItem.has_value() == false )
		{
			ImGui::End();
			return;
		}

		switch ( selectedItem.value().type )
		{
		case MikuEngine::SelectableType::ENTITY:
			InspectorPanel::RenderEntityInInspector( selectedItem.value(), editorLayer, scene );
			break;
		case MikuEngine::SelectableType::ASSET:
			InspectorPanel::RenderAssetInInspector( selectedItem.value(), editorLayer, scene );
			break;
		default:
			break;
		}

		ImGui::End();
	}

	void InspectorPanel::RenderEntityInInspector( const MikuEngine::SelectableItem& item, EditorLayer& editorLayer, MikuEngine::Scene& scene )
	{
		auto selectedEntityUUID = item.uuid;
		auto selectedEntity = scene.GetEntityByID( selectedEntityUUID );

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

				MikuEngine::TransformSystem::TransformComponentRenderImGui( selectedEntity.value(), transformC );
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::SpriteRendererComponent>() )
			{
				auto& spriteRendererC = selectedEntity.value().GetComponent<MikuEngine::SpriteRendererComponent>();

				std::function<void()> textureEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID ]() { editorLayer.m_TextureSelectionWindow.emplace_back( selectedEntityUUID ); };
				std::function<void()> shaderEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID ]() { editorLayer.m_ShaderSelectionWindow.emplace_back( selectedEntityUUID ); };

				MikuEngine::SpriteRendererSystem::SpriteRendererComponentRenderImGui( selectedEntity.value(), spriteRendererC, textureEditBtnCallback, shaderEditBtnCallback );
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::CameraComponent>() )
			{
				auto& cameraC = selectedEntity.value().GetComponent<MikuEngine::CameraComponent>();
				MikuEngine::CameraSystem::CameraComponentRenderImGui( selectedEntity.value(), cameraC );
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::NativeScriptComponent>() )
			{
				auto& nativeScriptC = selectedEntity.value().GetComponent<MikuEngine::NativeScriptComponent>();
				MikuEngine::ScriptExecutionSystem::NativeScriptComponentRenderImGui( selectedEntity.value(), nativeScriptC );
			}
		}

		if ( scene.GetSelectedItem().has_value() )
		{
			if ( MikuEngine::ImguiManager::FullWidthButton( "Add Component" ) ) ImGui::OpenPopup( "add-component-popup" );

			if ( ImGui::BeginPopup( "add-component-popup" ) )
			{
				if ( ImGui::Selectable( "CameraComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::CameraComponent>();
				if ( ImGui::Selectable( "SpriteRendererComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::SpriteRendererComponent>();
				if ( ImGui::Selectable( "NativeScriptComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::NativeScriptComponent>();

				ImGui::EndPopup();
			}
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
	}

	void InspectorPanel::RenderAssetInInspector( const MikuEngine::SelectableItem& item, EditorLayer& editorLayer, MikuEngine::Scene& scene )
	{
		auto assetPoolManager = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager();
		auto selectedAssetID = item.uuid;
		auto selectedAssetType = assetPoolManager.GetAssetTypeFromPool( selectedAssetID );

		switch ( selectedAssetType )
		{
		case MikuEngine::AssetType::NONE:
			break;
		case MikuEngine::AssetType::MATERIAL: {
			auto material = assetPoolManager.GetMaterialManager().GetMaterial( selectedAssetID );
			if ( material.has_value() == false ) return;
			material.value().RenderInspectorImGui();
			break;
		}
		case MikuEngine::AssetType::SHADER:
			break;
		case MikuEngine::AssetType::TEXTURE:
			break;
		case MikuEngine::AssetType::SCENE:
			break;
		case MikuEngine::AssetType::MODEL:
			break;
		}
	}
}
