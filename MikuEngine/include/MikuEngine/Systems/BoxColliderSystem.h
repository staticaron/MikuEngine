#pragma once

#include "Core.h"

#include "Entity.h"

namespace MikuEngine
{
	class MIKU_API BoxColliderSystem
	{
	public:
		static bool IsColliding( Entity entity1, Entity entity2 );

	private:
	};
}
