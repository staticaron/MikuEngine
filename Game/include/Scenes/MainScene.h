#pragma once

#include "Scene/Scene.h"

namespace RhythmGame
{
	class MainScene : public MikuEngine::Scene
	{
		void Render( const MikuEngine::Renderer& renderer ) const override;
	};
}
