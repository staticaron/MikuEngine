#include "NativeScripts/MoveEntityScript.h"

#include "Components.h"
#include "Logger.h"

namespace MikuEngine
{
	void MoveEntityScript::OnReady()
	{
		MIKU_CLIENT_INFO( "Move Entity Script Instantiated!" );
	}

	void MoveEntityScript::OnUpdate()
	{
		auto& transformComp = GetComponent<TransformComponent>();
		transformComp.Position.x += 1;
	}
}
