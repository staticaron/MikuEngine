#pragma once

#include <string>

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API DLLloader
	{
	public:
		DLLloader() = default;

		void LoadDLL( const std::string& dllLocation );

	private:
	};
}
