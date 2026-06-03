#pragma once

#include <filesystem>
#include <optional>

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
		bool DeSerialize( Scene& scene, std::optional<std::filesystem::path> sceneFilePath );

	private:
	};
}
