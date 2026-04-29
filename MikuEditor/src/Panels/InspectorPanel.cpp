#include "Panels/InspectorPanel.h"

#include "imgui.h"

#include "Component/MaterialComponent.h"
#include "Component/ShaderComponent.h"
#include "Components.h"
#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Data/SelectableItem.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Scene/Scene.h"
#include "MikuEngine/Systems/CameraSystem.h"
#include "MikuEngine/Systems/LightingSystem.h"
#include "MikuEngine/Systems/MeshRendererSystem.h"
#include "MikuEngine/Systems/ScriptExecutionSystem.h"
#include "MikuEngine/Systems/SpriteRendererSystem.h"
#include "MikuEngine/Systems/TransformSystems.h"

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

				ImGui::SameLine();

				if ( ImGui::Button( "< DEL >" ) ) scene.AddEntityToDeleteQueue( selectedEntityUUID );
			}

			// TRANSFORM
			if ( selectedEntity.value().HasComponent<MikuEngine::TransformComponent>() )
			{
				auto& transformC = selectedEntity.value().GetComponent<MikuEngine::TransformComponent>();

				MikuEngine::TransformSystem::TransformComponentRenderImGui( selectedEntity.value(), transformC );
			}

			// SPRITE RENDERER
			if ( selectedEntity.value().HasComponent<MikuEngine::SpriteRendererComponent>() )
			{
				auto& spriteRendererC = selectedEntity.value().GetComponent<MikuEngine::SpriteRendererComponent>();

				std::function<void()> textureEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID ]() { editorLayer.m_TextureSelectionWindow.emplace_back( selectedEntityUUID ); };
				std::function<void( MikuEngine::UUID itemUUID )> onitemSelected = [ &scene, selectedEntityUUID ]( MikuEngine::UUID itemUUID ) { scene.GetEntityByID( selectedEntityUUID ).value().GetComponent<MikuEngine::SpriteRendererComponent>().MaterialUUID = itemUUID; };
				std::function<void()> materialEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID, onitemSelected ]() { editorLayer.m_MaterialSelectionWindow.emplace_back( onitemSelected ); };

				MikuEngine::SpriteRendererSystem::SpriteRendererComponentRenderImGui( selectedEntity.value(), spriteRendererC, textureEditBtnCallback, materialEditBtnCallback );
			}

			// MESH RENDERER
			if ( selectedEntity.value().HasComponent<MikuEngine::MeshRendererComponent>() )
			{
				auto& meshRendererC = selectedEntity.value().GetComponent<MikuEngine::MeshRendererComponent>();

				std::function<void( MikuEngine::UUID )> onModelItemSelection = [ &scene, selectedEntityUUID ]( MikuEngine::UUID selectedModelUUID ) { scene.GetEntityByID( selectedEntityUUID ).value().GetComponent<MikuEngine::MeshRendererComponent>().ModelIdentifier = selectedModelUUID; };
				std::function<void()> modelEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID, &onModelItemSelection ]() { editorLayer.m_ModelSelectionWindow.emplace_back( onModelItemSelection ); };

				std::function<void( MikuEngine::UUID itemUUID )> onitemSelected = [ &scene, selectedEntityUUID ]( MikuEngine::UUID itemUUID ) { scene.GetEntityByID( selectedEntityUUID ).value().GetComponent<MikuEngine::MeshRendererComponent>().MaterialIdentifier = itemUUID; };
				std::function<void()> materialEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID, onitemSelected ]() { editorLayer.m_MaterialSelectionWindow.emplace_back( onitemSelected ); };

				MikuEngine::MeshRendererSystem::MeshRendererComponentRenderImGui( selectedEntity.value(), meshRendererC, modelEditBtnCallback, materialEditBtnCallback );
			}

			// CAMERA
			if ( selectedEntity.value().HasComponent<MikuEngine::CameraComponent>() )
			{
				auto& cameraC = selectedEntity.value().GetComponent<MikuEngine::CameraComponent>();
				MikuEngine::CameraSystem::CameraComponentRenderImGui( selectedEntity.value(), cameraC );
			}

			// NATIVE SCRIPT
			if ( selectedEntity.value().HasComponent<MikuEngine::NativeScriptComponent>() )
			{
				auto& nativeScriptC = selectedEntity.value().GetComponent<MikuEngine::NativeScriptComponent>();
				MikuEngine::ScriptExecutionSystem::NativeScriptComponentRenderImGui( selectedEntity.value(), nativeScriptC );
			}

			// DIRECTIONAL LIGHT
			if ( selectedEntity.value().HasComponent<MikuEngine::DirectionalLightComponent>() )
			{
				auto& directionalLightC = selectedEntity.value().GetComponent<MikuEngine::DirectionalLightComponent>();
				MikuEngine::LightingSystem::DirectionalLightComponentRenderImGui( selectedEntity.value(), directionalLightC );
			}
		}

		if ( scene.GetSelectedItem().has_value() )
		{
			if ( MikuEngine::ImguiManager::FullWidthButton( "Add Component" ) ) ImGui::OpenPopup( "add-component-popup" );

			if ( ImGui::BeginPopup( "add-component-popup" ) )
			{
				if ( ImGui::Selectable( "CameraComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::CameraComponent>();
				if ( ImGui::Selectable( "SpriteRendererComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::SpriteRendererComponent>();
				if ( ImGui::Selectable( "MeshRendererComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::MeshRendererComponent>();
				if ( ImGui::Selectable( "NativeScriptComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::NativeScriptComponent>();
				if ( ImGui::Selectable( "DirectionalLightComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::DirectionalLightComponent>();

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
		auto selectedItem = scene.GetSelectedItem();

		if ( selectedItem.has_value() == false ) return;

		if ( selectedItem.value().type != MikuEngine::SelectableType::ASSET ) return;

		auto& assetPoolManager = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager();

		switch ( item.assetType )
		{
		case MikuEngine::AssetType::NONE:
			break;
		case MikuEngine::AssetType::MATERIAL: {
			auto materialContainer = assetPoolManager.GetMaterialManager().GetMaterial( item.uuid );
			if ( materialContainer.has_value() == false ) return;

			MaterialComponent::RenderMaterialComponent( editorLayer, *materialContainer.value() );
			break;
		}
		case MikuEngine::AssetType::SHADER: {
			auto shaderContainer = assetPoolManager.GetShaderManager().GetShader( item.uuid );
			if ( shaderContainer.has_value() == false ) return;

			ShaderComponent::RenderShaderComponent( *shaderContainer.value() );
			break;
		}
		case MikuEngine::AssetType::TEXTURE:
			break;
		case MikuEngine::AssetType::SCENE:
			break;
		case MikuEngine::AssetType::MODEL:
			break;
		case MikuEngine::AssetType::FILE:
			break;
		case MikuEngine::AssetType::SCRIPT:
			break;
		default:
			break;
		}
	}
}
