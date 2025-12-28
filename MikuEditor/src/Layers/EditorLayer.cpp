#include "Layers/EditorLayer.h"

#include "AppLevelStuff.h"
#include "Panels/Panels.h"

namespace MikuEditor
{
	void EditorLayer::Update( double dt )
	{
		m_Scene.Update( dt );
	}

	void EditorLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		m_Scene.Render( appLevelStuff );
	}

	void EditorLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		MikuEditor::MenuBar::RenderMenuBar( m_Scene );
		MikuEditor::HierarchyPanel::RenderHierarchy( m_Scene );
		MikuEditor::InspectorPanel::RenderInspectorPanel( *this, m_Scene );
		MikuEditor::AssetBrowserPanel::RenderAssetBrowserPanel( m_Scene );

		m_Scene.RenderImGui( appLevelStuff );
	}
}
