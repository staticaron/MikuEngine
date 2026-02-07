#pragma once

#include <string>

#include "Core.h"
#include "Logger.h"
#include "ScriptRegistry.h"

extern "C" void RegisterScriptInEngine( std::string className, MikuEngine::ScriptCreatorFn scriptCreatorFn, MikuEngine::ScriptDestroyFn scriptDestroyFn );

#define REGISTER_SCRIPT( T, name )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
	static bool reg_##__LINE__ = []() {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
		RegisterScriptInEngine( name, []() -> MikuEngine::NativeScript* { return new T(); }, []( MikuEngine::NativeScript* instance ) { delete ( T* )instance; } );                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
		return true;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           \
	}();

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
