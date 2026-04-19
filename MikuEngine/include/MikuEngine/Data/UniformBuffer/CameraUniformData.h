#pragma once

#include "glm/ext/matrix_float4x4.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API CameraUniformData
	{
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		glm::vec4 position;
		glm::vec4 direction;

		bool operator==( const CameraUniformData& matrixData ) const { return ( matrixData.projectionMatrix == projectionMatrix ) && ( matrixData.viewMatrix == viewMatrix ) && ( matrixData.position == position ) && ( matrixData.direction == direction ); }
	};
}
