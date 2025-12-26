#pragma once

#include "Scene.h"

namespace MikuEngine
{
	class SceneUIElements
	{
	public:
		static void RenderMenuBar( Scene& scene );
		static void RenderHierarchy( Scene& scene );
		static void RenderProjectView( Scene& scene );
		static void RenderInspector( Scene& scene );

	private:
	};
}
