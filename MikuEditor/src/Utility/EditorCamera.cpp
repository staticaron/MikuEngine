#include "Utility/EditorCamera.h"

#include "Application.h"
#include "Input/Input.h"
#include "Layers/EditorLevelStuff.h"

namespace MikuEditor
{
	EditorCamera::EditorCamera()
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	void EditorCamera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective( glm::radians( 60.0f ), MikuEngine::Application::GetDataContainer().GetViewportAspectRatio(), 1.0f, 1000.0f );
	}

	void EditorCamera::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::translate( glm::mat4( 1.0f ), Position ) * glm::toMat4( GetOrientation() );
	}

	void EditorCamera::Update( double dt )
	{
		const auto& centralInput = MikuEngine::Application::GetAppLevelStuff().GetCentralInput();

		if ( centralInput.IsKeyPressed( GLFW_KEY_LEFT_ALT ) )
		{
			const auto& mouseDelta = centralInput.GetMousePositionDelta();

			m_Yaw += mouseDelta.x * m_RotationSpeed;
			m_Pitch += mouseDelta.y + m_RotationSpeed;

			UpdateViewMatrix();
		}
	}

	void EditorCamera::Translate( double dt )
	{
		const auto& [ xAxis, yAxis ] = MikuEngine::Input::GetAxisRaw();
		Position = { Position.x + xAxis * m_CameraSpeed * dt, Position.y + yAxis * m_CameraSpeed * dt, Position.z };
	}

	void EditorCamera::RenderImGui( EditorLevelStuff& editorLevelStuff )
	{
		bool& open = editorLevelStuff.IsEditorCameraEditorWindowOpen;

		if ( ImGui::Begin( "Editor Camera", &open ) == false ) return;

		ImGui::DragFloat3( "Position", &Position.x );
		ImGui::DragFloat3( "Rotation", &Rotation.x );
		ImGui::DragFloat( "Speed", &m_CameraSpeed );

		ImGui::End();
	}

}
