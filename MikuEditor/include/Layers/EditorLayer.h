#pragma once

#include "AppLevelStuff.h"
#include "MikuEngine/Layer.h"

namespace MikuEditor
{
	class EditorLayer : public MikuEngine::Layer
	{
	public:
		void Update( double dt ) override;
		void Render( MikuEngine::AppLevelStuff& appLevelStuff ) const override;
		void RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff ) override;

	private:
	};
}
