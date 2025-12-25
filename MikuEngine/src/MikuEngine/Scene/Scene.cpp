#include "Scene/Scene.h"

#include "AppLevelStuff.h"
#include "Components.h"
#include "Entity.h"
#include "Systems/RenderImGuiSystem.h"
#include "Systems/RenderingSystem.h"

namespace MikuEngine
{
	Scene::Scene()
	{
		/*
		// Create One Camera
		auto mainCam = CreateEntity( "Main Camera", this );
		mainCam.AddComponent<CameraComponent>();

		// Create Entity
		auto entity = CreateEntity( "Main GameObject", this );
		entity.GetComponent<TransformComponent>().Position = glm::vec3( 100.0f, 100.0f, 0.0f );
		entity.GetComponent<TransformComponent>().Scale = glm::vec3( 100.0f, 100.0f, 0.0f );

		entity.AddComponent<SpriteRendererComponent>();
		entity.GetComponent<SpriteRendererComponent>().TextureIdentifier = "miku";
		*/
	}

	void Scene::Update( double dt ) {}

	void Scene::Render( AppLevelStuff& appLevelStuff ) const
	{
		RenderingSystem::RenderSprite( m_Registry, appLevelStuff );
	}

	void Scene::RenderImGui( const AppLevelStuff& appLevelStuff )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "Scene" ) )
			{
				ImGui::Separator();
				if ( ImGui::MenuItem( "Save", "CTRL+S" ) ) Save( "main.miku" );
				ImGui::Separator();
				if ( ImGui::MenuItem( "Open", "CTRL+O" ) ) Load( "main.miku" );
				ImGui::Separator();
				if ( ImGui::MenuItem( "Clean", "CTRL+W" ) ) Clean();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		RenderImGuiSystem::RenderImGui( m_Registry, appLevelStuff );
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
