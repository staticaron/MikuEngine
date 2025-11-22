#include "Scenes/MainScene.h"

#include "Managers/TextureManager.h"

namespace RhythmGame
{
	MainScene::MainScene()
	{
		m_GOs.emplace_back();
	}

	MainScene::~MainScene()
	{
	}

	void MainScene::Update( double dt )
	{
	}

	void MainScene::Render( const MikuEngine::Renderer& renderer, const MikuEngine::TextureManager& textureManager ) const
	{
		for ( int x = 0; x < m_GOs.size(); x++ )
			m_GOs[ x ].Render( renderer, textureManager, m_Camera );
	}

	void MainScene::RenderImGui()
	{
	}
}
