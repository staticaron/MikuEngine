#include "MouseController.h"

#include "MikuEngine/Input/Input.h"

void MouseController::OnUpdate( double dt )
{
	auto [ mouseX, mouseY ] = MikuEngine::Input::GetMousePosition();
	bool isPrimaryPressed = MikuEngine::Input::IsMouseButtonPressed( 0 );

	auto& transformC = m_Entity->GetComponent<MikuEngine::TransformComponent>();

	if ( isPrimaryPressed ) transformC.Position = { mouseX, mouseY, 0 };
}
