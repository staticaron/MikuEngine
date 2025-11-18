#pragma once

#include "MikuEngine/Layer.h"

namespace RhythmGame
{
	class MainScene : public MikuEngine::Layer
	{
		void Render( const MikuEngine::Renderer& renderer ) const override;
	};
}
