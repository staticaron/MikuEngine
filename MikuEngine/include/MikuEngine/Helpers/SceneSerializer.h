#pragma once

#include <string>

#include "Core.h"

namespace MikuEngine
{
	class Scene;
}

namespace MikuEngine
{
	class MIKU_API SceneSerializer
	{
	public:
		SceneSerializer() = default;

		void Serialize( Scene& scene );
		bool DeSerialize( Scene& scene, const char* sceneFilePath = nullptr );

	private:
	};
}
