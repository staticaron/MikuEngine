#include "Layers/RendererLayer.h"

#include "MikuEngine/AppLevelStuff.h"

namespace MikuEditor
{
	void RendererLayer::Update( double dt ) {}
	void RendererLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const {}
	void RendererLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		ImGui::Begin( "Game" );

		ImGui::End();
	}
}
