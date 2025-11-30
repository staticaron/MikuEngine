#include "Layers/MainLayer.h"

#include "AppLevelStuff.h"

namespace RhythmGame
{
	void MainLayer::Update( double dt )
	{
		for ( int x = 0; x < m_Scenes.size(); x++ )
			m_Scenes[ x ]->Update( dt );
	}

	void MainLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		for ( int x = 0; x < m_Scenes.size(); x++ )
			m_Scenes[ x ]->Render( appLevelStuff );
	}

	void MainLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		for ( int x = 0; x < m_Scenes.size(); x++ )
			m_Scenes[ x ]->RenderImGui( appLevelStuff );
	}
}
