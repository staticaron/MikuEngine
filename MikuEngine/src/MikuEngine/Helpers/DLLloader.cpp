#include "Helpers/DLLloader.h"

#include "dylib/dylib.hpp"
#include "NativeScript.h"

extern "C" MikuEngine::NativeScript* Function();

namespace MikuEngine
{
	void DLLloader::LoadDLL( const std::string& dllLocation )
	{
		auto dll = dylib::library( dllLocation.c_str() );
	}
}
