#include "Layers/EditorLayer.h"

#include "AppLevelStuff.h"
#include "MikuEngine/Systems/RenderingSystem.h"
#include "Panels/Panels.h"

namespace MikuEditor
{
	void EditorLayer::Update( double dt )
	{
		m_Scene->Update( dt );
	}

	void EditorLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		if ( m_EditorLevelStuff.m_CurrentPlayModeState == PlayModeState::PLAYING )
			m_Scene->Render( appLevelStuff );
		else
		{
			MikuEngine::CameraData cameraData = { m_EditorCamera.GetViewMatrix() };
			m_Scene->RenderInEditor( appLevelStuff, cameraData );
		}
	}

	void EditorLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		if ( m_EditorLevelStuff.m_CurrentPlayModeState == PlayModeState::PLAYING )
		{
			MikuEditor::MenuBar::RenderMenuBar( *this, appLevelStuff, m_EditorLevelStuff, *m_Scene );
			MikuEditor::EditorOverlayPanel::RenderEditorOverlayPanel( *this, appLevelStuff, m_EditorLevelStuff, *m_Scene );
		}
		else
		{
			MikuEditor::MenuBar::RenderMenuBar( *this, appLevelStuff, m_EditorLevelStuff, *m_Scene );
			MikuEditor::HierarchyPanel::RenderHierarchy( *m_Scene );
			MikuEditor::InspectorPanel::RenderInspectorPanel( *this, appLevelStuff, *m_Scene );
			MikuEditor::AssetBrowserPanel::RenderAssetBrowserPanel( *m_Scene );
			MikuEditor::EditorOverlayPanel::RenderEditorOverlayPanel( *this, appLevelStuff, m_EditorLevelStuff, *m_Scene );
			MikuEditor::ViewportPanel::RenderViewportPanel( *m_Scene );

			std::vector<unsigned int> completedTextureWindows;
			completedTextureWindows.reserve( m_TextureSelectionWindow.size() );

			for ( size_t x = 0; x < m_TextureSelectionWindow.size(); x++ )
			{
				auto response = m_TextureSelectionWindow.at( x ).RenderTextureSelectionWindow( appLevelStuff, *m_Scene );

				if ( response == TextureSelectionWindowResponse::ERROR || response == TextureSelectionWindowResponse::COMPLETED || response == TextureSelectionWindowResponse::CLOSED )
				{
					completedTextureWindows.push_back( x );
				}
			}

			// Remove the texture selection windows that are completed!
			for ( size_t x = 0; x < completedTextureWindows.size(); x++ )
				m_TextureSelectionWindow.erase( m_TextureSelectionWindow.begin() + completedTextureWindows.at( x ) );

			m_Scene->RenderImGui( appLevelStuff );
		}
	}
}
