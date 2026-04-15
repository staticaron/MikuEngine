#pragma once

#include "glm/ext/matrix_float4x4.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API MatrixData
	{
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		bool operator==( const MatrixData& matrixData ) const { return ( matrixData.projectionMatrix == projectionMatrix ) && ( matrixData.viewMatrix == viewMatrix ); }
	};
}
