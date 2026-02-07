#include "Helpers/DLLloader.h"

#include "dylib.hpp"

extern "C" void RegisterScriptInEngine( std::string className, MikuEngine::ScriptCreatorFn scriptCreatorFn, MikuEngine::ScriptDestroyFn scriptDestroyFn )
{
	MikuEngine::ScriptRegistry::LoadedScripts[ className ] = scriptCreatorFn;
	MIKU_CORE_INFO( "Registered Script with class {}", className );
}

namespace MikuEngine
{
	void DLLloader::LoadDLL( const std::string& dllLocation )
	{
		auto dll = dylib::library( dllLocation.c_str() );
	}
}
