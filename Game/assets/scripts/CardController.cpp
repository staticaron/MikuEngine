#include "CardController.h"

#include "MikuEngine/Input/Input.h"

void CardController::OnReady()
{
	MIKU_CLIENT_INFO( "Card Controller Script Ready!" );
}

void CardController::OnUpdate( double dt )
{
	auto [ x, y ] = MikuEngine::Input::GetMousePosition();
}
