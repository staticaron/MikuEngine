#include "MouseController.h"

#include "Logger.h"
#include "MikuEngine/Input/Input.h"
#include "MikuEngine/Systems.h"

void MouseController::OnUpdate( double dt )
{
	bool isPrimaryPressed = MikuEngine::Input::IsMouseButtonPressed( 0 );

	if ( isPrimaryPressed )
	{
		auto [ mouseX, mouseY ] = MikuEngine::Input::GetMousePosition();
		auto& transformC = m_Entity->GetComponent<MikuEngine::TransformComponent>();

		auto mainCam = m_Entity->GetScene().GetMainCamera();

		const auto mainCamEntity = mainCam->first;
		const auto& mainCamComponent = mainCam->second;

		auto worldPosOfMouseClick = MikuEngine::CameraSystem::GetWorldPosFromPixelPosition( mainCamEntity, mainCamComponent, { mouseX, mouseY } );

		transformC.Position = { worldPosOfMouseClick.x, worldPosOfMouseClick.y, 0 };

		MIKU_CLIENT_INFO( "{}, {} - {}, {}", mouseX, mouseY, worldPosOfMouseClick.x, worldPosOfMouseClick.y );
	}
}
