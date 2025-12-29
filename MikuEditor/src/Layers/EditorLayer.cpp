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
		MikuEditor::InspectorPanel::RenderInspectorPanel( *this, appLevelStuff, m_Scene );
		MikuEditor::AssetBrowserPanel::RenderAssetBrowserPanel( m_Scene );

		std::vector<unsigned int> completedTextureWindows;
		completedTextureWindows.reserve( m_TextureSelectionWindow.size() );

		for ( size_t x = 0; x < m_TextureSelectionWindow.size(); x++ )
		{
			auto response = m_TextureSelectionWindow.at( x ).RenderTextureSelectionWindow( appLevelStuff, m_Scene );

			if ( response == TextureSelectionWindowResponse::ERROR || response == TextureSelectionWindowResponse::COMPLETED || response == TextureSelectionWindowResponse::CLOSED )
			{
				completedTextureWindows.push_back( x );
			}
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedTextureWindows.size(); x++ )
			m_TextureSelectionWindow.erase( m_TextureSelectionWindow.begin() + completedTextureWindows.at( x ) );

		m_Scene.RenderImGui( appLevelStuff );
	}
}
