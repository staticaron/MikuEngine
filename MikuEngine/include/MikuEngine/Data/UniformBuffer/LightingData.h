#pragma once

#include "glm/ext/vector_float3.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API LightingData
	{
		glm::vec3 color;
		float intensity;

		// bool operator==( const LightingData& lightingData ) const { return ( lightingData.position == position ) && ( lightingData.direction == direction ) && ( lightingData.intensity == intensity ) && ( lightingData.color == color ); }
		bool operator==( const LightingData& lightingData ) const { return ( lightingData.color == color ) && ( lightingData.intensity == intensity ); }
	};
}
