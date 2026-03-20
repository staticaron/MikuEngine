#pragma once

#include "Core.h"

#include "UniformBuffer.h"

namespace MikuEngine
{
	class MIKU_API EditorUniformBuffer : public UniformBuffer
	{
	public:
		void Init( unsigned int size ) override;
	};
}
