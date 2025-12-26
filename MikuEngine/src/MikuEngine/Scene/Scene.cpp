#include "Scene/Scene.h"

#include <optional>

#include "AppLevelStuff.h"
#include "Logger.h"

#include "Components.h"
#include "Entity.h"
#include "Scene/SceneUIElements.h"
#include "Systems/RenderingSystem.h"

namespace MikuEngine
{
	Scene::Scene() {}

	void Scene::Update( double dt ) {}

	void Scene::Render( AppLevelStuff& appLevelStuff ) const
	{
		RenderingSystem::RenderSprite( m_Registry, appLevelStuff );
	}

	void Scene::RenderImGui( const AppLevelStuff& appLevelStuff )
	{
		ImGui::ShowDemoWindow();

		SceneUIElements::RenderMenuBar( *this );
		SceneUIElements::RenderHierarchy( *this );
		SceneUIElements::RenderInspector( *this );
	}

	Entity Scene::CreateEntity( const std::string& name, Scene* parentScene )
	{
		auto entity = m_Registry.create();

		Entity entt( entity, parentScene );

		m_Registry.emplace<IDComponent>( entity );
		m_Registry.emplace<DataComponent>( entity, name );
		m_Registry.emplace<TransformComponent>( entity );

		return entt;
	}

	Entity Scene::LoadEntity( const std::string& name, UUID uuid, Scene* parentScene )
	{
		auto entity = m_Registry.create();

		Entity entt( entity, parentScene );

		m_Registry.emplace<IDComponent>( entity, uuid );
		m_Registry.emplace<DataComponent>( entity, name );
		m_Registry.emplace<TransformComponent>( entity );

		return entt;
	}

	std::optional<Entity> Scene::GetSelectedEntity()
	{
		auto entities = m_Registry.view<IDComponent>();

		for ( const auto& [ entity, id ] : entities.each() )
			if ( id.ID == m_SelectedEntityID ) return Entity( entity, this );

		return {};
	}

	std::vector<Entity> Scene::GetAllEntities()
	{
		std::vector<Entity> entities;

		auto entities_raw = m_Registry.view<entt::entity>();

		for ( auto entitiy_raw : entities_raw )
			entities.emplace_back( Entity{ entitiy_raw, this } );

		return entities;
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
