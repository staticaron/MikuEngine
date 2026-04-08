#include "Utility/EditorCamera.h"

#include "glm/gtx/quaternion.hpp"

#include "Application.h"
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
		m_ViewMatrix = glm::inverse( m_ViewMatrix );
	}

	void EditorCamera::Update( double dt )
	{
		const auto& centralInput = MikuEngine::Application::GetAppLevelStuff().GetCentralInput();

		if ( centralInput.IsMouseButtonPressed( GLFW_MOUSE_BUTTON_2 ) )
		{
			if ( m_MovementLocked == true ) centralInput.LockCursor();
			m_MovementLocked = false;
		}
		else if ( centralInput.IsMouseButtonPressed( GLFW_MOUSE_BUTTON_2 ) == false )
		{
			if ( m_MovementLocked == false ) centralInput.UnlockCursor();
			m_MovementLocked = true;
		}

		if ( m_MovementLocked != false ) return;

		Translate( dt );

		const auto& mouseDelta = centralInput.GetMousePositionDelta();

		m_Yaw += mouseDelta.x * m_RotationSpeed;
		m_Pitch -= mouseDelta.y * m_RotationSpeed;

		UpdateViewMatrix();
	}

	void EditorCamera::Translate( double dt )
	{
		const auto& centralInput = MikuEngine::Application::GetAppLevelStuff().GetCentralInput();
		const auto& axisRaw = centralInput.GetAxisRaw();

		float forwardMovement = axisRaw.y * dt * m_CameraSpeed;
		const auto& forwardDirection = GetForwardDirection();
		Position = { Position.x + forwardDirection.x * forwardMovement, Position.y + forwardDirection.y * forwardMovement, Position.z + forwardDirection.z * forwardMovement };

		float lateralMovement = axisRaw.x * dt * m_CameraSpeed;
		const auto& rightDirection = GetRightDirection();
		Position = { Position.x + rightDirection.x * lateralMovement, Position.y + rightDirection.y * lateralMovement, Position.z + rightDirection.z * lateralMovement };

		float verticalMovement = axisRaw.z * dt * m_CameraSpeed;
		const auto& upDirection = GetUpDirection();
		Position = { Position.x + upDirection.x * verticalMovement, Position.y + upDirection.y * verticalMovement, Position.z + upDirection.z * verticalMovement };
	}

	void EditorCamera::RenderImGui( EditorLevelStuff& editorLevelStuff )
	{
		bool& open = editorLevelStuff.IsEditorCameraEditorWindowOpen;

		if ( ImGui::Begin( "Editor Camera", &open ) == false ) return;

		ImGui::DragFloat3( "Position", &Position.x );
		ImGui::DragFloat( "Move Speed", &m_CameraSpeed );
		ImGui::DragFloat( "Rotation Speed", &m_RotationSpeed );

		ImGui::End();
	}
}
