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
		Entity( UUID uuid, entt::entity entity, Scene* parentScene );

		UUID GetUUID() const
		{
			auto idC = GetReadOnlyComponent<IDComponent>();
			return idC.ID;
		}

		void SetUUID( UUID uuid ) { GetOrAddComponent<IDComponent>().ID = uuid; }

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
		T GetReadOnlyComponent() const
		{
			return m_ParentScene->m_Registry.get<T>( m_Entity );
		}

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		T& GetOrAddComponent()
		{
			if ( HasComponent<T>() )
				return GetComponent<T>();
			else
			{
				AddComponent<T>();
				return GetComponent<T>();
			}
		}

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		bool HasComponent() const
		{
			return m_ParentScene->m_Registry.any_of<T>( m_Entity );
		}

		std::string GetNamedIdentifier() const;

	private:
		Scene* m_ParentScene = nullptr;
		entt::entity m_Entity;
	};
}
