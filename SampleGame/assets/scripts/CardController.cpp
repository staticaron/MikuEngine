#include "CardController.h"

#include "Application.h"
#include "MikuEngine/Systems.h"

void CardController::OnUpdate( double dt )
{
	const auto& centralInput = MikuEngine::Application::GetAppLevelStuff().GetCentralInput();

	bool isPrimaryPressed = centralInput.IsMouseButtonPressed( GLFW_MOUSE_BUTTON_1 );

	auto& transformC = m_Entity->GetComponent<MikuEngine::TransformComponent>();

	const auto& mousePos = centralInput.GetMousePosition();

	if ( isPrimaryPressed )
	{

		if ( m_WasPressedInPreviousFrame == false ) m_StartPosition = { mousePos.x, mousePos.y };

		glm::vec2 deltaRaw = glm::vec2{ mousePos.x, mousePos.y } - m_StartPosition;
		glm::vec2 delta = deltaRaw == glm::vec2{ 0.0f, 0.0f } ? glm::vec2{ 0.0f, 0.0f } : glm::normalize( deltaRaw );

		auto mainCam = m_Entity->GetScene().GetMainCamera();

		const auto mainCamEntity = mainCam->first;
		const auto& mainCamComponent = mainCam->second;

		float xRot = glm::clamp( static_cast<float>( transformC.Rotation.x + delta.y * m_RotateSpeed * dt ), m_MinMaxRotX.x, m_MinMaxRotX.y );
		float yRot = glm::clamp( static_cast<float>( transformC.Rotation.y + delta.x * m_RotateSpeed * dt ), m_MinMaxRotY.x, m_MinMaxRotY.y );
		float zRot = transformC.Rotation.x;

		transformC.Rotation = { transformC.Rotation.x - delta.y * m_RotateSpeed * dt, transformC.Rotation.y + delta.x * m_RotateSpeed * dt, 0.0f };

		m_StartPosition = { mousePos.x, mousePos.y };
	}
	else
	{
		if ( glm::abs( transformC.Rotation.x ) > 0.005f || glm::abs( transformC.Rotation.y ) > 0.005f )
		{
			transformC.Rotation = { glm::mix( transformC.Rotation.x, 0.0f, m_NormalizingSpeed * dt ), glm::mix( transformC.Rotation.y, 0.0f, m_NormalizingSpeed * dt ), transformC.Rotation.z };
		}
	}

	m_WasPressedInPreviousFrame = isPrimaryPressed;

	bool isSecondaryPressed = centralInput.IsMouseButtonPressed( GLFW_MOUSE_BUTTON_2 );

	if ( isSecondaryPressed )
	{
		auto mainCam = m_Entity->GetScene().GetMainCamera();

		const auto mainCamEntity = mainCam->first;
		const auto& mainCamComponent = mainCam->second;

		auto worldPosOfMouseClick = MikuEngine::CameraSystem::GetWorldPosFromPixelPosition( mainCamEntity, mainCamComponent, { mousePos.x, mousePos.y } );

		transformC.Position = { worldPosOfMouseClick.x, worldPosOfMouseClick.y, transformC.Position.z };
	}
}
