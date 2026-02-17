#pragma once

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class CameraData;
}

namespace MikuEngine
{
	class MIKU_API MeshRendererSystem
	{
	public:
		static void RenderMesh( const Scene& scene, const CameraData& cameraData );
	};
}
