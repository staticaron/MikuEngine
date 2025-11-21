#pragma once

#include "MikuEngine/Layer.h"
#include "Rendering/Renderer.h"

namespace RhythmGame
{
	class MainLayer : public MikuEngine::Layer
	{
	public:
		void Update( double dt ) override;
		void Render( const MikuEngine::Renderer& renderer ) const override;
		void RenderImgui() override;

	private:
	};
}
