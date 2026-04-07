#include "PlayerController.h"

#include "MikuEngine/Application.h"
#include "MikuEngine/Components.h"

void PlayerController::OnUpdate( double dt )
{
	auto& transformComponent = m_Entity->GetComponent<MikuEngine::TransformComponent>();
	const auto& axisRaw = MikuEngine::Application::GetAppLevelStuff().GetCentralInput().GetAxisRaw();

	transformComponent.Position.x += axisRaw.x * m_MoveSpeed;
	transformComponent.Position.y += axisRaw.y * m_MoveSpeed;
}
