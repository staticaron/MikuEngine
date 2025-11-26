#pragma once

#include "AppLevelStuff.h"
#include "Managers/TextureManager.h"
#include "MikuEngine/Layer.h"
#include "Rendering/Renderer.h"

namespace RhythmGame
{
	class MainLayer : public MikuEngine::Layer
	{
	public:
		MainLayer( MikuEngine::AppLevelStuff& appLevelStuff ) : MikuEngine::Layer( appLevelStuff ) {}

		void Update( double dt ) override;
		void Render( const MikuEngine::Renderer& renderer, const MikuEngine::TextureManager& textureManager ) const override;
		void RenderImgui() override;

	private:
	};
}
