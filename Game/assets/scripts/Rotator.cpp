#include "Rotator.h"

#include "MikuEngine/Components.h"

void Rotator::OnUpdate( double dt )
{
	auto& transformComponent = m_Entity->GetComponent<MikuEngine::TransformComponent>();

	transformComponent.Rotation.z += m_RotateSpeed * dt;
}
