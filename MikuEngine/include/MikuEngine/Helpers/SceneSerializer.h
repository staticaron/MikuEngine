#pragma once

#include <string>

#include "Core.h"

namespace MikuEngine
{
	class Scene;

	class MIKU_API SceneSerializer
	{
	public:
		SceneSerializer() = default;

		void Serialize( Scene& scene, const std::string& savePath );

		bool DeSerialize( Scene& scene, const std::string& scenepath );

	private:
	};
}
