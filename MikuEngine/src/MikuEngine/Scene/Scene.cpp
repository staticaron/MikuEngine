#include "Scene/Scene.h"

#include "AppLevelStuff.h"
#include "Logger.h"

#include "Components.h"
#include "Entity.h"
#include "Systems/RenderingSystem.h"

namespace MikuEngine
{
	Scene::Scene() {}

	void Scene::Update( double dt ) {}

	void Scene::Render( AppLevelStuff& appLevelStuff ) const
	{
		RenderingSystem::RenderSprite( m_Registry, appLevelStuff );
	}

	void Scene::RenderImGui( const AppLevelStuff& appLevelStuff ) {}

	Entity Scene::CreateEntity( const std::string& name, Scene* parentScene )
	{
		auto entity = m_Registry.create();

		Entity entt( ( UUID() ), entity, parentScene );

		m_Registry.emplace<DataComponent>( entity, name );
		m_Registry.emplace<TransformComponent>( entity );

		return entt;
	}

	Entity Scene::LoadEntity( const std::string& name, UUID uuid, Scene* parentScene )
	{
		auto entity = m_Registry.create();

		Entity entt( uuid, entity, parentScene );

		m_Registry.emplace<DataComponent>( entity, name );
		m_Registry.emplace<TransformComponent>( entity );

		return entt;
	}

	std::optional<Entity> Scene::GetSelectedEntity()
	{
		auto entities = m_Registry.view<IDComponent>();

		for ( const auto& [ entity, idC ] : entities.each() )
			if ( idC.ID == m_SelectedEntityID ) return Entity( idC.ID, entity, this );

		return {};
	}

	void Scene::SetSelectedEntity( UUID uuid )
	{
		if ( !GetEntityByID( uuid ).has_value() )
		{
			MIKU_CORE_ERROR( "This Entity is not present in scene! Can't set as active entity!" );
			return;
		}

		m_SelectedEntityID = uuid;
	}

	std::vector<Entity> Scene::GetAllEntities()
	{
		std::vector<Entity> entities;

		auto entities_raw = m_Registry.view<IDComponent>();

		for ( const auto [ entitiy_raw, idC ] : entities_raw.each() )
			entities.emplace_back( Entity{ idC.ID, entitiy_raw, this } );

		return entities;
	}

	std::optional<Entity> Scene::GetEntityByID( UUID id )
	{
		auto idView = m_Registry.view<IDComponent>();

		for ( const auto& [ entity, idC ] : idView.each() )
		{
			if ( idC.ID == id ) return Entity{ idC.ID, entity, this };
		}

		return {};
	}

	void Scene::Clean()
	{
		m_Registry.clear();
	}

	void Scene::Save( const std::string& savePath )
	{
		m_Serializer.Serialize( *this, savePath );
	}

	bool Scene::Load( const std::string& loadPath )
	{
		Clean();
		return m_Serializer.DeSerialize( *this, loadPath );
	}
}
