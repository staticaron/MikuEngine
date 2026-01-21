#pragma once

#include "MikuEngine/Core.h"

namespace MikuEngine
{
	class Scene;
}

namespace MikuEditor
{
	class MIKU_API HierarchyPanel
	{
	public:
		static void RenderHierarchy( MikuEngine::Scene& scene );
	};
}
