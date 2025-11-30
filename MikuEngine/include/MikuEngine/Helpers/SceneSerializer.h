#pragma once

#include <string>

#include "Core.h"

class MIKU_API Scene;

namespace MikuEngine
{
	class MIKU_API SceneSerializer
	{
	public:
		SceneSerializer() = default;

		void Serialize( const Scene& scene );

		void DeSerialize( const std::string& scenepath );

	private:
	};
}
