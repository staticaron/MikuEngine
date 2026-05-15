#pragma once

#include <cstdint>

#include "Components/BaseComponent.h"
#include "Core.h"

namespace MikuEngine
{
	class MIKU_API StencilWriterComponent : public BaseComponent
	{
	public:
		uint8_t WriteValue;

	private:
	};
}
