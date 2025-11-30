#pragma once

#include "entt/entt.hpp"

#include "Core.h"

#include "Components/BaseComponent.h"

namespace MikuEngine
{
	class MIKU_API Entity
	{
	public:
		Entity( entt::registry& registry, const std::string& entitiyName );

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		void AddComponent( entt::registry& registry )
		{
			registry.emplace<T>( m_Entity );
		}

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		T& GetComponent( entt::registry& registry )
		{
			return registry.get<T>( m_Entity );
		}

	private:
		entt::entity m_Entity;
	};
}
