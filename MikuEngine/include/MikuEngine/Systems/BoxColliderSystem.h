#pragma once

#include "Core.h"

namespace MikuEngine
{
	class Entity;
}

namespace MikuEngine
{
	class MIKU_API BoxColliderSystem
	{
	public:
		static bool IsColliding( Entity entity1, Entity entity2 );

	private:
	};
}
