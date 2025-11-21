#include "Layers/MainLayer.h"
#include "Rendering/Renderer.h"

namespace RhythmGame
{
	void MainLayer::Update( double dt )
	{
	}

	void MainLayer::Render( const MikuEngine::Renderer& renderer ) const
	{
		for ( int x = 0; x < m_Scenes.size(); x++ )
		{
			m_Scenes[ x ]->Render( renderer );
		}
	}

	void MainLayer::RenderImgui()
	{
	}
}
