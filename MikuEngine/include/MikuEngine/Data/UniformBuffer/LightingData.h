#pragma once

#include "glm/ext/vector_float4.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API LightingData
	{
		glm::vec4 position;
		glm::vec4 direction;
		glm::vec4 color;
		float intensity;

		bool operator==( const LightingData& lightingData ) const { return ( lightingData.direction == direction ) && ( lightingData.position == position ) && ( lightingData.color == color ) && ( lightingData.intensity == intensity ); }
	};
}
