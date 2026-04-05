#include "PlayerController.h"

#include "MikuEngine/Components.h"
#include "MikuEngine/Input/Input.h"

void PlayerController::OnUpdate( double dt )
{
	auto& transformComponent = m_Entity->GetComponent<MikuEngine::TransformComponent>();
	auto [ x, y ] = MikuEngine::Input::GetAxisRaw();

	transformComponent.Position.x += x * m_MoveSpeed;
	transformComponent.Position.y += y * m_MoveSpeed;
}
