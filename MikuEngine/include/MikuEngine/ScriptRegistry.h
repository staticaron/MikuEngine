#pragma once

#include <string>
#include <unordered_map>

#include "NativeScript.h"

namespace MikuEngine
{
	typedef MikuEngine::NativeScript* ( *ScriptCreatorFn )();
	typedef void ( *ScriptDestroyFn )( MikuEngine::NativeScript* );

	struct ScriptMetaData
	{
		std::string ClassName;

		ScriptCreatorFn CreatorFn;
		ScriptDestroyFn DestroyFn;
	};

	class ScriptRegistry
	{
	public:
		static std::unordered_map<std::string, ScriptMetaData> LoadedScripts;
		static void RefreshScripts();

	private:
	};
}
