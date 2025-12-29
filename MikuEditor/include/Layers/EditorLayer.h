#pragma once

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Layer.h"

#include "Windows/TextureSelectionWindow.h"

namespace MikuEditor
{
	class InspectorPanel;

	class EditorLayer : public MikuEngine::Layer
	{
	public:
		void Update( double dt ) override;
		void Render( MikuEngine::AppLevelStuff& appLevelStuff ) const override;
		void RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff ) override;

	private:
		std::vector<TextureSelectionWindow> m_TextureSelectionWindow;

		friend class InspectorPanel;
	};
}
