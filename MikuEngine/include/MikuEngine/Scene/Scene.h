#pragma once

#include <optional>
#include <vector>

#include "entt/entt.hpp"

#include "Core.h"
#include "UUID.h"

#include "AppLevelStuff.h"
#include "Helpers/SceneSerializer.h"

namespace MikuEngine
{
	class Entity;

	class MIKU_API Scene
	{
	public:
		Scene();
		~Scene() = default;

		void Update( double dt );
		void Render( AppLevelStuff& appLevelStuff ) const;
		void RenderImGui( const AppLevelStuff& appLevelStuff );

		Entity CreateEntity( const std::string& name, Scene* parentScene );
		Entity LoadEntity( const std::string& name, UUID uuid, Scene* parentScene );

		std::optional<Entity> GetSelectedEntity();
		void SetSelectedEntity( UUID uuid );

		std::vector<Entity> GetAllEntities();
		std::optional<Entity> GetEntityByID( UUID id );

		void Clean();
		void Save( const std::string& path );
		bool Load( const std::string& path );

	private:
		entt::registry m_Registry;
		SceneSerializer m_Serializer;

		UUID m_SelectedEntityID;

	protected:
		friend class SceneSerializer;
		friend class Entity;
	};
}
