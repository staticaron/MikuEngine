#pragma once

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

		void Clean();
		void Save( const std::string& path );
		bool Load( const std::string& path );

	private:
	protected:
		entt::registry m_Registry;

		SceneSerializer m_Serializer;

		friend class SceneSerializer;
		friend class Entity;
	};
}
