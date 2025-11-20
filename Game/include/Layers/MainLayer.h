#pragma once

#include "MikuEngine/Layer.h"
#include "Rendering/Renderer.h"

namespace RhythmGame
{
	class MainLayer : public MikuEngine::Layer
	{
		void Render( const MikuEngine::Renderer& renderer ) const override
		{
			m_Scene->Render( renderer );
		}
	};
}
