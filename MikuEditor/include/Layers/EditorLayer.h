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
		EditorLayer( MikuEngine::Scene* scene );

		static EditorLayer* GetEditorLayer();

		void Update( double dt ) override;
		void Render( MikuEngine::AppLevelStuff& appLevelStuff ) const override;
		void RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff ) override;
		const EditorLevelStuff& GetEditorLayerInfo() const { return m_EditorLevelStuff; }

	private:
		EditorLevelStuff m_EditorLevelStuff;
		EditorCamera m_EditorCamera;

		std::vector<TextureSelectionWindow> m_TextureSelectionWindow;

		bool m_IsViewportPanelFocused = false;

		friend class InspectorPanel;
	};
}
