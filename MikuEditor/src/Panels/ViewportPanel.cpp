#include "Panels/ViewportPanel.h"

#include "Components/TransformComponent.h"
#include "glm/ext.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "ImGuizmo.h"

#include "Entity.h"
#include "Input/CentralInput.h"
#include "Layers/EditorLayer.h"
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
		bool CtrlPressed = centralInput.IsKeyPressed( GLFW_KEY_LEFT_CONTROL );

		if ( TPressed && m_CurrentOperation != ImGuizmo::TRANSLATE ) m_CurrentOperation = ImGuizmo::TRANSLATE;
		if ( RPressed && m_CurrentOperation != ImGuizmo::ROTATE ) m_CurrentOperation = ImGuizmo::ROTATE;
		if ( XPressed && m_CurrentOperation != ImGuizmo::SCALE ) m_CurrentOperation = ImGuizmo::SCALE;
		if ( GPressed && m_CurrentOperation != ImGuizmo::UNIVERSAL ) m_CurrentOperation = ImGuizmo::UNIVERSAL;

		if ( CtrlPressed && m_IsSnapping == false ) m_IsSnapping = true;
		if ( !CtrlPressed && m_IsSnapping == true ) m_IsSnapping = false;
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

					// SAVE OLD ROTATION FOR DELTA INCREMENT LATER
					glm::mat4 pureRotationMtx = transformMtx;

					pureRotationMtx[ 0 ] = glm::normalize( pureRotationMtx[ 0 ] );
					pureRotationMtx[ 1 ] = glm::normalize( pureRotationMtx[ 1 ] );
					pureRotationMtx[ 2 ] = glm::normalize( pureRotationMtx[ 2 ] );

					glm::vec3 oldRotation = {};
					glm::extractEulerAngleXYZ( pureRotationMtx, oldRotation.x, oldRotation.y, oldRotation.z );

					auto& editorCam = editorLayer.GetEditorCamera();

					// FLIP THE Y BECAUSE WE ARE NOT USING THE DEFAULT +Y IS UP
					auto projectionMatrix = editorCam.GetProjMatrix();
					projectionMatrix[ 1 ][ 1 ] *= -1.0f;

					// SNAPPING
					auto snapValue = glm::vec3( 0.01f );
					if ( m_IsSnapping )
					{
						snapValue = glm::vec3( 1.0f );
						if ( m_CurrentOperation == ImGuizmo::ROTATE ) snapValue = glm::vec3( 15.0f );
					}

					// RENDER HANDLE GIZMO
					ImGuizmo::Manipulate( glm::value_ptr( editorCam.GetViewMatrix() ), glm::value_ptr( projectionMatrix ), m_CurrentOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr( transformMtx ), nullptr, glm::value_ptr( snapValue ) );

					// WHEN THE HANDLE IS DOWN, APPLY THE UPDATED VALUES
					if ( ImGuizmo::IsUsing() )
					{
						// FETCH THE TRANSLATION AND SCALE FROM THE UPDATED MATRIX
						glm::vec3 translation, rotationDegrees, scale;
						ImGuizmo::DecomposeMatrixToComponents( glm::value_ptr( transformMtx ), glm::value_ptr( translation ), glm::value_ptr( rotationDegrees ), glm::value_ptr( scale ) );

						// APPLY THE UPDATED TRANSLATION AND SCALE
						transform.Position = translation;
						transform.Scale = scale;

						// FETCH ROTATION DIRECTLY FROM MATRIX ( NOT USING DECOMPOSE AS IT USES YXZ )
						glm::mat4 pureRotationMtx = transformMtx;

						pureRotationMtx[ 0 ] = glm::normalize( pureRotationMtx[ 0 ] );
						pureRotationMtx[ 1 ] = glm::normalize( pureRotationMtx[ 1 ] );
						pureRotationMtx[ 2 ] = glm::normalize( pureRotationMtx[ 2 ] );

						glm::vec3 eulerAngles = {};
						glm::extractEulerAngleXYZ( pureRotationMtx, eulerAngles.x, eulerAngles.y, eulerAngles.z );
						glm::vec3 deltaRot = eulerAngles - oldRotation;

						// APPLY THE ROTATION
						transform.Rotation += deltaRot;
					}
				}
			}
		}

		ImGui::End();

		return isFocused;
	}
}
