#pragma once

#include "Core.h"
#include "NativeScript.h"
#include "ScriptRegistry.h"

namespace MikuEngine
{
	struct MIKU_API NativeScriptComponent : public BaseComponent
	{
		std::string ScriptIdentifier = "SampleClass";

		NativeScript* Instance = nullptr;

		void Instantiate() { Instance = ScriptRegistry::RegisteredScripts.at( ScriptIdentifier ).CreatorFn(); }
		void DeAllocate() { ScriptRegistry::RegisteredScripts.at( ScriptIdentifier ).DestroyFn( Instance ); }

		void Update( Entity entity )
		{
			if ( Instance ) Instance->OnUpdate( entity );
		}

		void OnReady( Entity entity )
		{
			if ( Instance ) Instance->OnReady( entity );
		}

		void OnDestroy( Entity entity )
		{
			if ( Instance ) Instance->OnDestroy( entity );
		}
	};
}
