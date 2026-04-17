#include "Panels/ViewportPanel.h"

#include "Components/TransformComponent.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "ImGuizmo.h"

#include "Entity.h"
#include "Input/CentralInput.h"
#include "Layers/EditorLayer.h"
#include "Logger.h"
#include "MikuEngine/Application.h"
#include "MikuEngine/Scene/Scene.h"
#include "Systems/CameraSystem.h"

namespace MikuEditor
{
	void ViewportPanel::Update()
	{
		const auto& centralInput = MikuEngine::Application::GetAppLevelStuff().GetCentralInput();

		bool TPressed = centralInput.IsKeyPressed( GLFW_KEY_T );
		bool RPressed = centralInput.IsKeyPressed( GLFW_KEY_R );
		bool XPressed = centralInput.IsKeyPressed( GLFW_KEY_X );
		bool GPressed = centralInput.IsKeyPressed( GLFW_KEY_G );

		if ( TPressed && m_CurrentOperation != ImGuizmo::TRANSLATE ) m_CurrentOperation = ImGuizmo::TRANSLATE;
		if ( RPressed && m_CurrentOperation != ImGuizmo::ROTATE ) m_CurrentOperation = ImGuizmo::ROTATE;
		if ( XPressed && m_CurrentOperation != ImGuizmo::SCALE ) m_CurrentOperation = ImGuizmo::SCALE;
		if ( GPressed && m_CurrentOperation != ImGuizmo::UNIVERSAL ) m_CurrentOperation = ImGuizmo::UNIVERSAL;
	}

	bool ViewportPanel::RenderViewportPanel( EditorLayer& editorLayer, MikuEngine::Scene& scene )
	{
		bool isFocused = false;

		if ( ImGui::Begin( "Viewport" ) )
		{
			if ( ImGui::IsWindowFocused() ) isFocused = true;

			ImVec2 windowSize = ImGui::GetContentRegionAvail();

			glm::vec2 viewPortSize = MikuEngine::Application::GetDataContainer().GetViewportSize();

			auto& frameBuffer = MikuEngine::Application::GetApplication()->GetSceneFBO();

			if ( windowSize.x != viewPortSize.x || windowSize.y != viewPortSize.y )
			{
				frameBuffer.ResizeBufferTexture( { windowSize.x, windowSize.y } );
			}

			ImGui::Image( ( void* )( intptr_t )frameBuffer.GetTextureID(), { windowSize.x, windowSize.y }, { 0, 1 }, { 1, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } );

			ImGuizmo::Enable( true );
			ImGuizmo::SetOrthographic( false );
			ImGuizmo::SetDrawlist( ImGui::GetWindowDrawList() );

			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();

			ImGuiIO& io = ImGui::GetIO();
			ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight );

			auto selectedEntityUUID = scene.GetSelectedItem();
			if ( selectedEntityUUID.has_value() == true && selectedEntityUUID->type == MikuEngine::SelectableType::ENTITY )
			{
				auto selectedEntity = scene.GetEntityByID( selectedEntityUUID->uuid );
				if ( selectedEntity.has_value() )
				{
					auto& transform = selectedEntity->GetComponent<MikuEngine::TransformComponent>();
					auto transformMtx = transform.GetModelMatrix();
					auto oldRotation = transform.Rotation;

					auto& editorCam = editorLayer.GetEditorCamera();

					auto projectionMatrix = editorCam.GetProjMatrix();
					projectionMatrix[ 1 ][ 1 ] *= -1.0f;

					ImGuizmo::Manipulate( glm::value_ptr( editorCam.GetViewMatrix() ), glm::value_ptr( projectionMatrix ), m_CurrentOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr( transformMtx ) );

					if ( ImGuizmo::IsUsing() )
					{
						glm::vec3 translation, rotation, scale;

						ImGuizmo::DecomposeMatrixToComponents( glm::value_ptr( transformMtx ), glm::value_ptr( translation ), glm::value_ptr( rotation ), glm::value_ptr( scale ) );

						const glm::vec3 deltaRotation = rotation - transform.Rotation;

						MIKU_CLIENT_INFO( "Rotation Delta {} {} {} ", deltaRotation.x, deltaRotation.y, deltaRotation.z );

						transform.Position = translation;
						transform.Rotation += glm::radians( deltaRotation );
						transform.Scale = scale;
					}
				}
			}
		}

		ImGui::End();

		return isFocused;
	}
}
