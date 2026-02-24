#pragma once

#include "MikuEngine/Layer.h"

#include "EditorLevelStuff.h"
#include "Panels/AssetBrowserPanel.h"
#include "Utility/EditorCamera.h"
#include "Windows/ShaderSelectionWindow.h"
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

		void ManageTextureSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff );
		void ManageShaderSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff );

	private:
		EditorLevelStuff m_EditorLevelStuff;
		EditorCamera m_EditorCamera;

		std::vector<TextureSelectionWindow> m_TextureSelectionWindow;
		std::vector<ShaderSelectionWindow> m_ShaderSelectionWindow;

		bool m_IsViewportPanelFocused = false;

		// Panels
		AssetBrowserPanel m_AssetBrowserPanel;

		friend class InspectorPanel;
	};
}
