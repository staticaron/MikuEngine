#include "Panels/InspectorPanel.h"

#include "imgui.h"

#include "Component/MaterialComponent.h"
#include "Component/ShaderComponent.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Helpers/ImGuiHelper.h"
#include "MikuEngine/Scene/Scene.h"
#include "MikuEngine/Systems.h"

#include "Layers/EditorLayer.h"
#include "Panels/Panels.h"

namespace MikuEditor
{
	void InspectorPanel::RenderInspectorPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Inspector" );

		auto selectedEntity = scene.GetSelectedEntity();
		auto selectedAsset = editorLayer.GetSeletedAsset();

		if ( selectedEntity.has_value() ) InspectorPanel::RenderEntityInInspector( selectedEntity.value(), editorLayer, scene );
		if ( selectedAsset.has_value() ) InspectorPanel::RenderAssetInInspector( selectedAsset.value().uuid, editorLayer, scene );

		ImGui::End();
	}

	void InspectorPanel::RenderFolderImGui( const std::filesystem::path& folderPath )
	{
		char folderName[ 255 ];
		const std::string folderNameFromPath = folderPath.stem();
		std::copy( folderNameFromPath.begin(), folderNameFromPath.begin() + folderNameFromPath.length(), folderName );
		folderName[ folderNameFromPath.length() ] = '\0';

		// render the entity name
		MikuEngine::ImGuiHelper::RenderLabel( "Folder" );
		ImGui::InputText( "##FolderName", folderName, 255 );

		if ( ImGui::IsItemDeactivatedAfterEdit() )
		{
			if ( std::strcmp( folderName, folderNameFromPath.c_str() ) != 0 )
			{
				const std::filesystem::path newFolderPath = folderPath.parent_path() / folderName;
				std::filesystem::rename( folderPath, newFolderPath );
			}
		}
	}

	void InspectorPanel::RenderEntityInInspector( const MikuEngine::UUID& selectedEntityUUID, EditorLayer& editorLayer, MikuEngine::Scene& scene )
	{
		auto selectedEntity = scene.GetEntityByID( selectedEntityUUID );

		if ( selectedEntity.has_value() )
		{
			if ( selectedEntity.value().HasComponent<MikuEngine::DataComponent>() )
			{
				auto& dataC = selectedEntity.value().GetComponent<MikuEngine::DataComponent>();

				// create a container to hold the name of the entity
				char entityName[ 255 ];
				std::copy( dataC.EntityName.begin(), dataC.EntityName.begin() + dataC.EntityName.length(), entityName );
				entityName[ dataC.EntityName.length() ] = '\0';

				// render the entity name
				MikuEngine::ImGuiHelper::RenderLabel( "Entity" );
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

				std::function<void( MikuEngine::UUID itemUUID )> onTextureSelected = [ &scene, selectedEntityUUID ]( MikuEngine::UUID itemUUID ) { scene.GetEntityByID( selectedEntityUUID ).value().GetComponent<MikuEngine::SpriteRendererComponent>().TextureIdentifier = itemUUID; };
				std::function<void()> textureEditBtnCallback = [ &editorLayer, selectedEntityUUID, &onTextureSelected ]() { editorLayer.m_TextureSelectionWindow.emplace_back( onTextureSelected ); };
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

			// SKYBOX RENDERER
			if ( selectedEntity.value().HasComponent<MikuEngine::SkyboxComponent>() )
			{
				auto& skyboxC = selectedEntity.value().GetComponent<MikuEngine::SkyboxComponent>();

				std::function<void( MikuEngine::UUID itemUUID )> onModelItemSelected = [ &scene, selectedEntityUUID ]( MikuEngine::UUID itemUUID ) { scene.GetEntityByID( selectedEntityUUID ).value().GetComponent<MikuEngine::SkyboxComponent>().ModelIdentifier = itemUUID; };
				std::function<void()> modelEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID, onModelItemSelected ]() { editorLayer.m_ModelSelectionWindow.emplace_back( onModelItemSelected ); };

				std::function<void( MikuEngine::UUID itemUUID )> onMaterialItemSelected = [ &scene, selectedEntityUUID ]( MikuEngine::UUID itemUUID ) { scene.GetEntityByID( selectedEntityUUID ).value().GetComponent<MikuEngine::SkyboxComponent>().MaterialIdentifier = itemUUID; };
				std::function<void()> materialEditBtnCallback = [ &editorLayer, &scene, selectedEntityUUID, onMaterialItemSelected ]() { editorLayer.m_MaterialSelectionWindow.emplace_back( onMaterialItemSelected ); };

				MikuEngine::SkyboxRendererSystem::SkyboxComponentRenderImGui( selectedEntity.value(), skyboxC, modelEditBtnCallback, materialEditBtnCallback );
			}

			// STENCIL READER
			if ( selectedEntity.value().HasComponent<MikuEngine::StencilReaderComponent>() )
			{
				auto& stencilReaderC = selectedEntity.value().GetComponent<MikuEngine::StencilReaderComponent>();
				MikuEngine::StencilSystem::StencilReaderRenderImGui( selectedEntity.value(), stencilReaderC );
			}

			// STENCIL WRITER
			if ( selectedEntity.value().HasComponent<MikuEngine::StencilWriterComponent>() )
			{
				auto& stencilWriterC = selectedEntity.value().GetComponent<MikuEngine::StencilWriterComponent>();
				MikuEngine::StencilSystem::StencilWriterRenderImGui( selectedEntity.value(), stencilWriterC );
			}

			// PARTICLE SYSTEM
			if ( selectedEntity.value().HasComponent<MikuEngine::ParticleEmitterComponent>() )
			{
				auto& particleSystemC = selectedEntity.value().GetComponent<MikuEngine::ParticleEmitterComponent>();
				MikuEngine::ParticleEmitterSystem::ParticleEmitterComponentRenderImGui( selectedEntity.value(), particleSystemC );
			}
		}

		if ( scene.GetSelectedEntity().has_value() )
		{
			if ( MikuEngine::ImguiManager::FullWidthButton( "Add Component" ) ) ImGui::OpenPopup( "add-component-popup" );

			if ( ImGui::BeginPopup( "add-component-popup" ) )
			{
				if ( ImGui::Selectable( "CameraComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::CameraComponent>();
				if ( ImGui::Selectable( "SpriteRendererComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::SpriteRendererComponent>();
				if ( ImGui::Selectable( "MeshRendererComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::MeshRendererComponent>();
				if ( ImGui::Selectable( "NativeScriptComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::NativeScriptComponent>();
				if ( ImGui::Selectable( "DirectionalLightComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::DirectionalLightComponent>();
				if ( ImGui::Selectable( "SkyboxComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::SkyboxComponent>();
				if ( ImGui::Selectable( "StencilReaderComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::StencilReaderComponent>();
				if ( ImGui::Selectable( "StencilWriterComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::StencilWriterComponent>();
				if ( ImGui::Selectable( "ParticleSystemComponent" ) ) selectedEntity.value().AddComponent<MikuEngine::ParticleEmitterComponent>();

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

	void InspectorPanel::RenderAssetInInspector( const MikuEngine::UUID& selectedEntityUUID, EditorLayer& editorLayer, MikuEngine::Scene& scene )
	{
		auto selectedAsset = editorLayer.GetSeletedAsset();

		if ( selectedAsset.has_value() == false ) return;

		auto& assetPoolManager = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager();

		switch ( selectedAsset.value().assetType )
		{
		case MikuEngine::AssetType::NONE:
			break;

		case MikuEngine::AssetType::FOLDER:
			InspectorPanel::RenderFolderImGui( selectedAsset->assetPath );
			break;

		case MikuEngine::AssetType::MATERIAL: {
			auto materialContainer = assetPoolManager.GetMaterialManager().GetMaterial( selectedAsset.value().uuid );
			if ( materialContainer.has_value() == false ) return;

			MaterialComponent::RenderMaterialComponent( editorLayer, *materialContainer.value() );
			break;
		}
		case MikuEngine::AssetType::SHADER: {
			auto shaderContainer = assetPoolManager.GetShaderManager().GetShader( selectedAsset.value().uuid );
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
