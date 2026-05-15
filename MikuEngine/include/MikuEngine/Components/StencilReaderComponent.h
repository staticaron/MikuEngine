#pragma once

#include <cstdint>

#include "BaseComponent.h"
#include "Core.h"

namespace MikuEngine
{
	class MIKU_API StencilReaderComponent : public BaseComponent
	{
	public:
		uint8_t ReadValue;

	private:
	};
}
