#include "Layers/MainLayer.h"

#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

namespace RhythmGame
{
	void MainLayer::Update( double dt )
	{
	}

	void MainLayer::Render( const MikuEngine::Renderer& renderer, const MikuEngine::TextureManager& textureManager ) const
	{
		for ( int x = 0; x < m_Scenes.size(); x++ )
			m_Scenes[ x ]->Render( renderer, textureManager );
	}

	void MainLayer::RenderImgui()
	{
	}
}
