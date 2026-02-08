#include "Scene/Scene.h"

#include "AppLevelStuff.h"
#include "Logger.h"

#include "Components.h"
#include "Entity.h"
#include "Systems/CameraSystem.h"
#include "Systems/RenderingSystem.h"
#include "Systems/ScriptExecutionSystem.h"

namespace MikuEngine
{
	Scene::Scene() {}

	void Scene::Update( double dt )
	{
		ScriptExecutionSystem::ExecuteScripts( *this );
	}

	void Scene::Render( AppLevelStuff& appLevelStuff ) const
	{
		auto mainCamera = GetMainCamera();

		if ( mainCamera.has_value() == false )
		{
			MIKU_CORE_WARN( "There is no active MainCamera." );
			return;
		}

		auto mainCameraEntity = mainCamera->first;
		auto mainCameraComponent = mainCamera->second;

		RenderingSystem::RenderSprite( *this, appLevelStuff, { CameraSystem::GetViewMatrix( mainCameraEntity ), mainCameraComponent.m_Resolution } );
	}

	void Scene::RenderInEditor( AppLevelStuff& appLevelStuff, const CameraData& cameraData ) const
	{
		RenderingSystem::RenderSprite( *this, appLevelStuff, cameraData );
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

		auto& dataC = m_Registry.emplace<DataComponent>( entity, name );
		auto& transformC = m_Registry.emplace<TransformComponent>( entity );

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

	std::optional<Entity> Scene::GetEntityFromEntt( entt::entity entity )
	{
		auto& idC = m_Registry.get<IDComponent>( entity );

		return {
		    { idC.ID, entity, this }
		     };
	}

	std::optional<std::pair<const Entity, const CameraComponent&>> Scene::GetMainCamera() const
	{
		auto cameraComponentView = m_Registry.view<IDComponent, CameraComponent>();

		for ( auto [ entity, idComponent, cameraComponent ] : cameraComponentView.each() )
		{
			if ( cameraComponent.m_IsMainCamera == false ) continue;

			return {
			    { { idComponent.ID, entity, const_cast<Scene*>( this ) }, cameraComponent }
			};
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
		MIKU_CORE_INFO( "Load" );
		return m_Serializer.DeSerialize( *this, loadPath );
	}
}
