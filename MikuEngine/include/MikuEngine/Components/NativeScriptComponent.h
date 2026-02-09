#pragma once

#include "Core.h"
#include "NativeScript.h"
#include "ScriptRegistry.h"

namespace MikuEngine
{
	struct MIKU_API NativeScriptComponent : public BaseComponent
	{
		std::string ScriptIdentifier = "";

		NativeScript* Instance = nullptr;

		bool Instantiate( const Entity& entity )
		{
			auto scriptMetaData = ScriptRegistry::RegisteredScripts.find( ScriptIdentifier );

			if ( scriptMetaData == ScriptRegistry::RegisteredScripts.end() ) return false;

			Instance = ScriptRegistry::RegisteredScripts.at( ScriptIdentifier ).CreatorFn();
			Instance->m_Entity = entity;

			return true;
		}

		void DeAllocate() { ScriptRegistry::RegisteredScripts.at( ScriptIdentifier ).DestroyFn( Instance ); }

		void Update( double dt )
		{
			if ( Instance ) Instance->OnUpdate( dt );
		}

		void OnReady()
		{
			if ( Instance ) Instance->OnReady();
		}

		void OnDestroy()
		{
			if ( Instance ) Instance->OnDestroy();
		}
	};
}
