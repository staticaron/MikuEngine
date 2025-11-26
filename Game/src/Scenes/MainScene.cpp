#include "Scenes/MainScene.h"

#include "AppLevelStuff.h"
#include "Components/SpriteRenderer.h"
#include "Components/Transform.h"
#include "Systems/RenderingSystem.h"

namespace RhythmGame
{
	MainScene::MainScene()
	{
		auto newGO = m_Registry.create();
		m_Registry.emplace<MikuEngine::TransformComponent>( newGO );
		m_Registry.emplace<MikuEngine::SpriteRendererComponent>( newGO );
	}

	MainScene::~MainScene() {}

	void MainScene::Update( double dt ) {}

	void MainScene::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		MikuEngine::RenderingSystem::RenderSprite( m_Registry, appLevelStuff );
	}

	void MainScene::RenderImGui() {}
}
