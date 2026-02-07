#include "ScriptRegistry.h"

namespace MikuEngine
{
	std::unordered_map<std::string, ScriptCreatorFn> ScriptRegistry::LoadedScripts;
}
