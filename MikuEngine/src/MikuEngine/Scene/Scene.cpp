#include "Scene/Scene.h"

#include "entt/entt.hpp"

#include "AppLevelStuff.h"
#include "Logger.h"

#include "Components.h"
#include "Data/CameraData.h"
#include "Entity.h"
#include "Systems/CameraSystem.h"
#include "Systems/MeshRendererSystem.h"
#include "Systems/ScriptExecutionSystem.h"
#include "Systems/SkyboxRendererSystem.h"
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
		auto& gameFBO = MikuEngine::Application::GetApplication()->GetGameFBO();
		gameFBO.Bind();

		auto& renderer = MikuEngine::Application::GetAppLevelStuff().GetRenderer();
		renderer.GetUniformBufferManager().GetGameUniformBuffer().Bind();     // Bind Matrix Data at 0
		renderer.GetUniformBufferManager().GetLightingUniformBuffer().Bind(); // Bind Lighting Data at 1

		renderer.ClearColor();

		auto mainCamera = GetMainCamera();
		if ( GetMainCamera().has_value() == false ) return;
		auto mainCameraEntity = mainCamera->first;
		auto mainCameraComponent = mainCamera->second;
		auto cameraData = CameraData{ CameraSystem::GetViewMatrix( mainCameraEntity ), CameraSystem::GetProjMatrix( mainCameraComponent ) };

		RunSystems( appLevelStuff, cameraData );

		gameFBO.UnBind();
	}

	void Scene::RenderInEditor( AppLevelStuff& appLevelStuff, const CameraData& editorCameraData ) const
	{
		auto& sceneFBO = MikuEngine::Application::GetApplication()->GetSceneFBO();
		sceneFBO.Bind();

		auto& renderer = MikuEngine::Application::GetAppLevelStuff().GetRenderer();

		renderer.GetUniformBufferManager().GetEditorUniformBuffer().Bind();   // Bind Matrix Data at 0
		renderer.GetUniformBufferManager().GetLightingUniformBuffer().Bind(); // Bind Lighting Data at 1
		renderer.ClearColor();

		RunSystems( appLevelStuff, editorCameraData );

		sceneFBO.UnBind();
	}

	void Scene::RunSystems( AppLevelStuff& appLevelStuff, const CameraData& cameraData ) const
	{
		SkyboxRendererSystem::RenderSkybox( *this, appLevelStuff, cameraData );
		MeshRendererSystem::RenderMesh( *this, appLevelStuff, cameraData );
		SpriteRendererSystem::RenderSprite( *this, appLevelStuff, cameraData );
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

	void Scene::DeleteEntity( const UUID& uuid )
	{
		auto entity = GetEntityByID( uuid );

		if ( entity.has_value() == false )
		{
			MIKU_CORE_WARN( "Entity you are trying to delete doesn't exists!" );
			return;
		}

		m_Registry.destroy( entity.value().GetEntt() );
		MIKU_CORE_WARN( "Entity Deleted" );
	}

	void Scene::PerformDeletions()
	{
		for ( auto entity : m_DeleteQueue )
			DeleteEntity( entity );

		m_DeleteQueue.clear();
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

	std::optional<std::pair<const Entity, const DirectionalLightComponent&>> Scene::GetMainLight() const
	{
		auto directionalLightView = m_Registry.view<IDComponent, DirectionalLightComponent>();

		for ( auto [ entity, idComponent, directionalLightC ] : directionalLightView.each() )
		{
			return {
			    { { idComponent.ID, entity, const_cast<Scene*>( this ) }, directionalLightC }
			  };
		}

		return {};
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

	void Scene::Clean()
	{
		m_Registry.clear();
	}

	void Scene::CopyFrom( const Scene& scene )
	{
		m_Registry.clear();

		const entt::registry& srcRegistry = scene.GetRegistry();

		auto view = srcRegistry.view<IDComponent>();

		// RECREATE THE ENTITIES
		for ( auto [ entity, idC ] : view.each() )
			static_cast<void>( m_Registry.create( entity ) );

		// RECREATE THE COMPONENTS
		srcRegistry.view<IDComponent>().each( [ & ]( auto entity, const IDComponent& tranformC ) { m_Registry.emplace_or_replace<IDComponent>( entity, tranformC ); } );
		srcRegistry.view<DataComponent>().each( [ & ]( auto entity, const DataComponent& tranformC ) { m_Registry.emplace_or_replace<DataComponent>( entity, tranformC ); } );
		srcRegistry.view<TransformComponent>().each( [ & ]( auto entity, const TransformComponent& tranformC ) { m_Registry.emplace_or_replace<TransformComponent>( entity, tranformC ); } );
		srcRegistry.view<SpriteRendererComponent>().each( [ & ]( auto entity, const SpriteRendererComponent& tranformC ) { m_Registry.emplace_or_replace<SpriteRendererComponent>( entity, tranformC ); } );
		srcRegistry.view<MeshRendererComponent>().each( [ & ]( auto entity, const MeshRendererComponent& tranformC ) { m_Registry.emplace_or_replace<MeshRendererComponent>( entity, tranformC ); } );
		srcRegistry.view<CameraComponent>().each( [ & ]( auto entity, const CameraComponent& tranformC ) { m_Registry.emplace_or_replace<CameraComponent>( entity, tranformC ); } );
		srcRegistry.view<DirectionalLightComponent>().each( [ & ]( auto entity, const DirectionalLightComponent& tranformC ) { m_Registry.emplace_or_replace<DirectionalLightComponent>( entity, tranformC ); } );
		srcRegistry.view<NativeScriptComponent>().each( [ & ]( auto entity, const NativeScriptComponent& tranformC ) { m_Registry.emplace_or_replace<NativeScriptComponent>( entity, tranformC ); } );
		srcRegistry.view<SkyboxComponent>().each( [ & ]( auto entity, const SkyboxComponent& tranformC ) { m_Registry.emplace_or_replace<SkyboxComponent>( entity, tranformC ); } );
		srcRegistry.view<StencilReaderComponent>().each( [ & ]( auto entity, const StencilReaderComponent& tranformC ) { m_Registry.emplace_or_replace<StencilReaderComponent>( entity, tranformC ); } );
		srcRegistry.view<StencilWriterComponent>().each( [ & ]( auto entity, const StencilWriterComponent& tranformC ) { m_Registry.emplace_or_replace<StencilWriterComponent>( entity, tranformC ); } );
	}
}
