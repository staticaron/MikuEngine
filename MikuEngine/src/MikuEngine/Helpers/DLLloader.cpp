#include "Helpers/DLLloader.h"

#include "dylib.hpp"

extern "C" void RegisterScriptInEngine( std::string className, MikuEngine::ScriptCreatorFn scriptCreatorFn, MikuEngine::ScriptDestroyFn scriptDestroyFn )
{
	MikuEngine::ScriptRegistry::RegisteredScripts[ className ] = { className, scriptCreatorFn, scriptDestroyFn };
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
