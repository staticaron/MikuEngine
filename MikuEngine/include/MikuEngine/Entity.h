#pragma once

#include "entt/entt.hpp"

#include "Core.h"

#include "Components/BaseComponent.h"
#include "Components/DataComponent.h"
#include "Components/IDComponent.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	class MIKU_API Entity
	{
	public:
		Entity( UUID uuid, entt::entity entity, Scene* parentScene, const std::string& name, std::optional<UUID> parentUUID = std::nullopt );

		UUID GetUUID() const
		{
			auto idC = GetReadOnlyComponent<IDComponent>();
			return idC.ID;
		}

		std::optional<UUID> GetParent() const
		{
			auto dataC = GetReadOnlyComponent<DataComponent>();
			return dataC.ParentUUID;
		}

		const entt::entity& GetEntt() const { return m_Entity; }

		void SetUUID( UUID uuid ) { GetOrAddComponent<IDComponent>().ID = uuid; }
		void SetParent( std::optional<UUID> uuid ) { GetOrAddComponent<DataComponent>().ParentUUID = uuid; }

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		void AddComponent()
		{
			m_ParentScene->m_Registry.emplace<T>( m_Entity );
		}

		template <typename T>
			requires( std::is_base_of_v<BaseComponent, T> )
		void RemoveComponent()
		{
			m_ParentScene->m_Registry.remove<T>( m_Entity );
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

		Scene& GetScene() { return *m_ParentScene; }

		std::string GetNamedIdentifier() const;

	private:
		Scene* m_ParentScene = nullptr;
		entt::entity m_Entity;
	};
}
