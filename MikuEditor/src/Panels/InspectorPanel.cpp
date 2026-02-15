#include "Panels/InspectorPanel.h"

#include "Components.h"
#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Logger.h"
#include "MikuEngine/Scene/Scene.h"
#include "MikuEngine/Systems/CameraSystem.h"
#include "MikuEngine/Systems/RenderingSystem.h"
#include "MikuEngine/Systems/ScriptExecutionSystem.h"
#include "MikuEngine/Systems/TransformSystems.h"

#include "imgui.h"
#include "Layers/EditorLayer.h"
#include "Panels/Panels.h"

namespace MikuEditor
{
	void InspectorPanel::RenderInspectorPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
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

				MikuEngine::TransformSystem::TransformComponentRenderImGui( selectedEntity.value(), transformC );
			}

			if ( selectedEntity.value().HasComponent<MikuEngine::SpriteRendererComponent>() )
			{
				auto& spriteRendererC = selectedEntity.value().GetComponent<MikuEngine::SpriteRendererComponent>();

				std::function<void()> textureEditBtnCallback = [ &editorLayer, &scene ]() { editorLayer.m_TextureSelectionWindow.emplace_back( scene.GetSelectedEntity().value().GetUUID() ); };

				std::function<void()> shaderEditBtnCallback = [ &editorLayer, &scene ]() { editorLayer.m_ShaderSelectionWindow.emplace_back( scene.GetSelectedEntity().value().GetUUID() ); };

				MikuEngine::RenderingSystem::SpriteRendererComponentRenderImGui( selectedEntity.value(), spriteRendererC, textureEditBtnCallback, shaderEditBtnCallback );
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

		// Show Add Component Button ( ACTIVE if scene has selected entity otherwise DISABLED)
		if ( scene.GetSelectedEntity().has_value() )
		{
			if ( MikuEngine::ImguiManager::FullWidthButton( "Add Component" ) ) ImGui::OpenPopup( "add-component-popup" );

			if ( ImGui::BeginPopup( "add-component-popup" ) )
			{

				if ( ImGui::Selectable( "CameraComponent" ) ) scene.GetSelectedEntity().value().AddComponent<MikuEngine::CameraComponent>();
				if ( ImGui::Selectable( "SpriteRendererComponent" ) ) scene.GetSelectedEntity().value().AddComponent<MikuEngine::SpriteRendererComponent>();
				if ( ImGui::Selectable( "NativeScriptComponent" ) ) scene.GetSelectedEntity().value().AddComponent<MikuEngine::NativeScriptComponent>();

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

		ImGui::End();
	}
}
