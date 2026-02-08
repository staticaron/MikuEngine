#include "ScriptRegistry.h"

#include "Helpers/DLLloader.h"

namespace MikuEngine
{
	std::unordered_map<std::string, ScriptMetaData> ScriptRegistry::RegisteredScripts;

	void ScriptRegistry::RefreshScripts()
	{
		RegisteredScripts.clear();
		DLLloader::LoadGameLogicDLL();
	}

}
