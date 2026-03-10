#include "Helpers/DLLloader.h"

#include "dylib.hpp"

#include "Logger.h"

extern "C" void RegisterScriptInEngine( std::string className, MikuEngine::ScriptCreatorFn scriptCreatorFn, MikuEngine::ScriptDestroyFn scriptDestroyFn )
{
	MikuEngine::ScriptRegistry::RegisteredScripts[ className ] = { className, scriptCreatorFn, scriptDestroyFn };
	MIKU_CORE_DEBUG( "Registered Script with class {}", className );
}

namespace MikuEngine
{
	static std::unique_ptr<dylib::library> LOADED_GAME_LOGIC_DLL = nullptr;

	void DLLloader::LoadGameLogicDLL()
	{
		LOADED_GAME_LOGIC_DLL = std::make_unique<dylib::library>( dylib::library( GAME_LOGIC_DLL_PATH ) );
	}

	void DLLloader::UnLoadGameLogicDLL()
	{
		LOADED_GAME_LOGIC_DLL.reset();
	}
}
