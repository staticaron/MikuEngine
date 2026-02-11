#include "MouseController.h"

#include "MikuEngine/Input/Input.h"

void MouseController::OnUpdate( double dt )
{
	bool isPrimaryPressed = MikuEngine::Input::IsMouseButtonPressed( 0 );

	if ( isPrimaryPressed )
	{
		auto [ mouseX, mouseY ] = MikuEngine::Input::GetMousePosition();
		auto& transformC = m_Entity->GetComponent<MikuEngine::TransformComponent>();

		transformC.Position = { mouseX, mouseY, 0 };
		MIKU_CLIENT_INFO( "Entity Position : {}, {}", mouseX, mouseY );
	}
}
