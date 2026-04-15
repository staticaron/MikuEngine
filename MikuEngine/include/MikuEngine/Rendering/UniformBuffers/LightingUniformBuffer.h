#pragma once

#include "Core.h"
#include "UniformBuffer.h"

namespace MikuEngine
{

	class MIKU_API LightingUniformBuffer : public UniformBuffer
	{
	public:
		void Init( unsigned int bindIndex, unsigned int size ) override;
	};
}
