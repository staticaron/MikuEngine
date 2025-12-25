#pragma once

#include "entt/entt.hpp"

#include "Core.h"
#include "UUID.h"

#include "Components.h"
#include "Scene/Scene.h"

namespace MikuEngine
{

	class MIKU_API Entity
	{
	public:
		Entity( entt::entity entity, Scene* parentScene );
		Entity( UUID uuid );

		UUID GetUUID() const { return m_UUID; }

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		void AddComponent()
		{
			m_ParentScene->m_Registry.emplace<T>( m_Entity );
		}

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		T& GetComponent()
		{
			return m_ParentScene->m_Registry.get<T>( m_Entity );
		}

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		bool HasComponent()
		{
			return m_ParentScene->m_Registry.any_of<T>( m_Entity );
		}

	private:
		Scene* m_ParentScene = nullptr;
		entt::entity m_Entity;

		UUID m_UUID;
	};
}
