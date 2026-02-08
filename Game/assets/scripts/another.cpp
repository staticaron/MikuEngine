#include "another.h"

#include "MikuEngine.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Input/Input.h"

void AnotherClass::OnUpdate( MikuEngine::Entity entity )
{
	auto& transformComponent = entity.GetComponent<MikuEngine::TransformComponent>();

	auto [ x, y ] = MikuEngine::Input::GetAxisRaw();
	transformComponent.Position.x += x;
	transformComponent.Position.y += y;
}
