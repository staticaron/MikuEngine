#include "Scene/Scene.h"

#include "AppLevelStuff.h"
#include "Logger.h"

#include "Components.h"
#include "Data/CameraData.h"
#include "Entity.h"
#include "Systems/CameraSystem.h"
#include "Systems/MeshRendererSystem.h"
#include "Systems/ScriptExecutionSystem.h"
#include "Systems/SpriteRendererSystem.h"

namespace MikuEngine
{
	Scene::Scene() {}

	void Scene::Update( double dt )
	{
		ScriptExecutionSystem::ExecuteScripts( *this );
	}

	void Scene::Render( AppLevelStuff& appLevelStuff ) const
	{
		// Bind the game frame buffer object; this will be used to render into the game view
		auto& gameFBO = MikuEngine::Application::GetApplication()->GetGameFBO();
		gameFBO.Bind();

		auto& renderer = MikuEngine::Application::GetAppLevelStuff().GetRenderer();
		renderer.ClearColor( { 0.23, 0.24f, 0.25f, 1.0f } );

		// Render nothing if camera is not there
		if ( GetMainCamera().has_value() == false ) return;

		renderer.GetUniformBufferManager().GetGameUniformBuffer().Bind();

		// Camera details for camera creating proj view matrices
		auto mainCamera = GetMainCamera();
		auto mainCameraEntity = mainCamera->first;
		auto mainCameraComponent = mainCamera->second;

		// Render the sprites
		SpriteRendererSystem::RenderSprite( *this, appLevelStuff, CameraData{ CameraSystem::GetViewMatrix( mainCameraEntity ), CameraSystem::GetProjMatrix( mainCameraComponent ) } );
		MeshRendererSystem::RenderMesh( *this, appLevelStuff, CameraData{ CameraSystem::GetViewMatrix( mainCameraEntity ), CameraSystem::GetProjMatrix( mainCameraComponent ) } );

		gameFBO.UnBind();
	}

	void Scene::RenderInEditor( AppLevelStuff& appLevelStuff, const CameraData& cameraData ) const
	{
		auto& sceneFBO = MikuEngine::Application::GetApplication()->GetSceneFBO();
		sceneFBO.Bind();

		auto& renderer = MikuEngine::Application::GetAppLevelStuff().GetRenderer();

		renderer.GetUniformBufferManager().GetEditorUniformBuffer().Bind();
		renderer.ClearColor( { 0.23f, 0.24f, 0.25f, 1.0f } );

		SpriteRendererSystem::RenderSprite( *this, appLevelStuff, cameraData );
		MeshRendererSystem::RenderMesh( *this, appLevelStuff, cameraData );

		sceneFBO.UnBind();
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

	std::optional<SelectableItem> Scene::GetSelectedItem()
	{
		return m_SelectedItem;
	}

	void Scene::SetSelectedItem( UUID uuid, SelectableType type, AssetType assetType )
	{
		if ( type == SelectableType::ENTITY && !GetEntityByID( uuid ).has_value() )
		{
			MIKU_CORE_ERROR( "This Entity is not present in scene! Can't set as active entity!" );
			return;
		}

		m_SelectedItem = { uuid, type, assetType };
		MIKU_CORE_INFO( "Selected Item set to {} ", uuid.ToString() );
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
			if ( cameraComponent.IsMainCamera() == false ) continue;

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

	void Scene::Save()
	{
		m_Serializer.Serialize( *this );
	}

	bool Scene::Load( const char* sceneFilePath )
	{
		Clean();
		return m_Serializer.DeSerialize( *this, sceneFilePath );
	}
}
