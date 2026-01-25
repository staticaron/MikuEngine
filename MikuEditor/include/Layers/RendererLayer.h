#pragma once

#include "MikuEngine/Layer.h"

#include "Windows/TextureSelectionWindow.h"

namespace MikuEngine
{
	class AppLevelStuff;
	class SceneFBO;
}

namespace MikuEditor
{
	class InspectorPanel;
}

namespace MikuEditor
{
	class RendererLayer : public MikuEngine::Layer
	{
	public:
		RendererLayer( MikuEngine::Scene* scene ) : MikuEngine::Layer( scene ) {}

		void Update( double dt ) override;
		void Render( MikuEngine::AppLevelStuff& appLevelStuff ) const override;
		void RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff ) override;

		void RenderFrameBuffer( const MikuEngine::AppLevelStuff& appLevelStuff );

	private:
	};
}
