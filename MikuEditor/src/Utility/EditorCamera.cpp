#include "Utility/EditorCamera.h"

#include "Application.h"
#include "Input/Input.h"
#include "Layers/EditorLevelStuff.h"

namespace MikuEditor
{
	glm::mat4 EditorCamera::GetProjMatrix() const
	{
		if ( m_IsPerspective == false )
			return glm::ortho( 0.0f, MikuEngine::Application::GetDataContainer().GetViewportSize().x, MikuEngine::Application::GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
		else
			return glm::perspective( glm::radians( 60.0f ), MikuEngine::Application::GetDataContainer().GetViewportAspectRatio(), 1.0f, 1000.0f );
	}

	glm::mat4 EditorCamera::GetViewMatrix() const
	{
		auto transform = glm::translate( glm::mat4( 1.0f ), Position );

		auto rotationX = glm::rotate( glm::mat4( 1.0f ), Rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		auto rotationY = glm::rotate( glm::mat4( 1.0f ), Rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		auto rotationZ = glm::rotate( glm::mat4( 1.0f ), Rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		auto rotation = rotationX * rotationY * rotationZ;
		return glm::inverse( transform * rotation );
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
