#include "Layers/MainLayer.h"

#include "AppLevelStuff.h"

namespace RhythmGame
{
	void MainLayer::Update( double dt ) {}

	void MainLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		for ( int x = 0; x < m_Scenes.size(); x++ )
			m_Scenes[ x ]->Render( appLevelStuff );
	}

	void MainLayer::RenderImgui() {}
}
