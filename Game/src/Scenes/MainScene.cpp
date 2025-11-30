#include "Scenes/MainScene.h"

#include "AppLevelStuff.h"
#include "Components/CameraComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Entity.h"
#include "Systems/RenderImGuiSystem.h"
#include "Systems/RenderingSystem.h"

namespace RhythmGame
{
	MainScene::MainScene()
	{
		// Create Scene Camera
		auto cameraEntity = MikuEngine::Entity( m_Registry, "Main Camera" );
		cameraEntity.AddComponent<MikuEngine::CameraComponent>( m_Registry );

		// Create Entity
		auto newGO = MikuEngine::Entity( m_Registry, "MyGO" );
		newGO.GetComponent<MikuEngine::TransformComponent>( m_Registry ).Scale = glm::vec3( 100.0f );
		newGO.AddComponent<MikuEngine::SpriteRendererComponent>( m_Registry );

		m_SceneSerializer.Serialize();
	}

	MainScene::~MainScene() {}

	void MainScene::Update( double dt ) {}

	void MainScene::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		MikuEngine::RenderingSystem::RenderSprite( m_Registry, appLevelStuff );
	}

	void MainScene::RenderImGui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		MikuEngine::RenderImGuiSystem::RenderImGui( m_Registry, appLevelStuff );
	}
}
