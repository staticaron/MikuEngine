#include "Scenes/MainScene.h"

#include "Components/SpriteRenderer.h"
#include "Components/Transform.h"
#include "Managers/TextureManager.h"
#include "Systems/RenderingSystem.h"

namespace RhythmGame
{
	MainScene::MainScene()
	{
		m_GOs.emplace_back();

		auto newGO = m_Registry.create();
		m_Registry.emplace<MikuEngine::TransformComponent>( newGO );
		m_Registry.emplace<MikuEngine::SpriteRenderer>( newGO );
	}

	MainScene::~MainScene() {}

	void MainScene::Update( double dt ) {}

	void MainScene::Render( const MikuEngine::Renderer& renderer, const MikuEngine::TextureManager& textureManager ) const
	{
		for ( int x = 0; x < m_GOs.size(); x++ )
			m_GOs[ x ].Render( renderer, textureManager, m_Camera );

		MikuEngine::RenderingSystem::RenderSprite( m_Registry );
	}

	void MainScene::RenderImGui() {}
}
