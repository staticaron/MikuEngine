#pragma once

#include <functional>

#include "Core.h"
#include "NativeScript.h"

namespace MikuEngine
{
	struct MIKU_API NativeScriptComponent : public BaseComponent
	{
		NativeScript* Instance = nullptr;

		NativeScriptComponent() = default;

		std::function<void()> Instantiate;
		std::function<void()> DeInstantiate;

		// Calling these on the native script component, will call the corresponding function on the ScriptableEntity
		std::function<void( Entity entity )> OnCreate;
		std::function<void( Entity entity )> OnReady;
		std::function<void( Entity entity )> OnUpdate;
		std::function<void( Entity entity )> OnDestroy;

		template <typename T>
			requires( std::is_base_of_v<NativeScript, T> )
		void Bind()
		{
			Instantiate = [ & ]() { Instance = new T(); };
			DeInstantiate = [ & ]() {
				delete ( T* )Instance;
				Instance = nullptr;
			};

			OnCreate = [ this ]( Entity entity ) {
				if ( this->Instance ) ( ( T* )this->Instance )->OnCreate( entity );
			};
			OnReady = [ this ]( Entity entity ) {
				if ( this->Instance ) ( ( T* )this->Instance )->OnReady( entity );
			};
			OnUpdate = [ this ]( Entity entity ) {
				if ( this->Instance ) ( ( T* )this->Instance )->OnUpdate( entity );
			};
			OnDestroy = [ this ]( Entity entity ) {
				if ( this->Instance ) ( ( T* )this->Instance )->OnDestroy( entity );
			};
		}
	};
}
