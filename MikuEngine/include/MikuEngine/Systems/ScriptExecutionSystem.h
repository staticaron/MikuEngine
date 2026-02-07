#pragma once

#include "Core.h"

namespace MikuEngine
{
	class Scene;
}

namespace MikuEngine
{
	struct MIKU_API ScriptExecutionSystem
	{
		static void ExecuteScripts( Scene& scene );
	};
}
