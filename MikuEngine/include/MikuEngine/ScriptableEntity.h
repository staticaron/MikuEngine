#pragma once

#include <type_traits>

#include "Components/BaseComponent.h"
#include "Core.h"
#include "Entity.h"

namespace MikuEngine
{
	class MIKU_API ScriptableEntity
	{
	public:
		ScriptableEntity( Entity entity ) : m_Entity( entity ) {}

		virtual void OnCreate() {};
		virtual void OnReady() {};
		virtual void OnUpdate() {};
		virtual void OnDestroy() {};

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity;
	};
}
