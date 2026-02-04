#pragma once

#include "MikuEngine/Layer.h"

#include "EditorLevelStuff.h"
#include "Utility/EditorCamera.h"
#include "Windows/TextureSelectionWindow.h"

namespace MikuEngine
{
	class AppLevelStuff;
}
namespace MikuEditor
{
	class InspectorPanel;
}

namespace MikuEditor
{
	class EditorLayer : public MikuEngine::Layer
	{
	public:
		EditorLayer( MikuEngine::Scene* scene ) : MikuEngine::Layer( scene ) {};

		void Update( double dt ) override;
		void Render( MikuEngine::AppLevelStuff& appLevelStuff ) const override;
		void RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff ) override;

	private:
		EditorLevelStuff m_EditorLevelStuff;
		EditorCamera m_EditorCamera;

		std::vector<TextureSelectionWindow> m_TextureSelectionWindow;

		bool m_IsViewportPanelFocused = false;

		friend class InspectorPanel;
	};
}
