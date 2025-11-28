#include "Scenes/MainScene.h"

#include "AppLevelStuff.h"
#include "Components/SpriteRenderer.h"
#include "Components/Transform.h"
#include "Systems/RenderImGuiSystem.h"
#include "Systems/RenderingSystem.h"

namespace RhythmGame
{
	MainScene::MainScene()
	{
		auto newGO = m_Registry.create();

		MikuEngine::TransformComponent transformComponent;
		transformComponent.Position = glm::vec3( 0.0f, 0.0f, 0.0f );
		transformComponent.Rotation = glm::vec3( 0.0f, 0.0f, 0.0f );
		transformComponent.Scale = glm::vec3( 100.0f, 100.0f, 0.0f );

		m_Registry.emplace<MikuEngine::TransformComponent>( newGO, transformComponent );
		m_Registry.emplace<MikuEngine::SpriteRendererComponent>( newGO );
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
