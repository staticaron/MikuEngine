#include "NativeScripts/MoveEntityScript.h"

#include "Logger.h"

namespace MikuEngine
{
	void MoveEntityScript::OnReady( Entity entity ) {}

	void MoveEntityScript::OnUpdate( Entity entity )
	{
		MIKU_CORE_INFO( "Running Update Function for entity named {}", entity.GetNamedIdentifier() );
	}
}
