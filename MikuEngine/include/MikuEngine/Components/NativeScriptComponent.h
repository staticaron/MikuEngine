#pragma once

#include <functional>

#include "Core.h"
#include "ScriptableEntity.h"

namespace MikuEngine
{
	struct MIKU_API NativeScriptComponent : public BaseComponent
	{
		ScriptableEntity* Instance = nullptr;

		NativeScriptComponent() = default;

		std::function<void()> Instantiate;
		std::function<void()> DeInstantiate;

		// Calling these on the native script component, will call the corresponding function on the ScriptableEntity
		std::function<void()> OnCreate;
		std::function<void()> OnReady;
		std::function<void()> OnUpdate;
		std::function<void()> OnDestroy;

		template <typename T>
			requires( std::is_base_of_v<ScriptableEntity, T> )
		void Bind( Entity entity )
		{
			Instantiate = [ this, entity ]() { this->Instance = new T( entity ); };
			DeInstantiate = [ this ]() {
				delete ( T* )this->Instance;
				this->Instance = nullptr;
			};

			OnCreate = [ this ]() {
				if ( this->Instance ) ( ( T* )this->Instance )->OnCreate();
			};
			OnReady = [ this ]() {
				if ( this->Instance ) ( ( T* )this->Instance )->OnReady();
			};
			OnUpdate = [ this ]() {
				if ( this->Instance ) ( ( T* )this->Instance )->OnUpdate();
			};
			OnDestroy = [ this ]() {
				if ( this->Instance ) ( ( T* )this->Instance )->OnDestroy();
			};
		}
	};
}
