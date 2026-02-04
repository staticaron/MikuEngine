#include "NativeScripts/MoveEntityScript.h"

#include "Components.h"

namespace MikuEngine
{
	void MoveEntityScript::OnReady() {}

	void MoveEntityScript::OnUpdate()
	{
		auto& transformComp = GetComponent<TransformComponent>();
		transformComp.Position.x += 0.1;
	}
}
