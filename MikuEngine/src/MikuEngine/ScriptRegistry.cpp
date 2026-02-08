#include "ScriptRegistry.h"

#include "Helpers/DLLloader.h"

namespace MikuEngine
{
	std::unordered_map<std::string, ScriptMetaData> ScriptRegistry::LoadedScripts;

	void ScriptRegistry::RefreshScripts()
	{
		LoadedScripts.clear();
		DLLloader::LoadGameLogicDLL();
	}
}
