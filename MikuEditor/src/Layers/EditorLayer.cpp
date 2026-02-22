#include "Layers/EditorLayer.h"

#include "AppLevelStuff.h"
#include "Application.h"
#include "Data/CameraData.h"
#include "MikuEngine/Systems/SpriteRendererSystem.h"
#include "Panels/Panels.h"
#include "Windows/WindowResponse.h"

namespace MikuEditor
{
	static EditorLayer* s_EditorLayer;

	EditorLayer::EditorLayer( MikuEngine::Scene* scene ) : MikuEngine::Layer( scene )
	{
		s_EditorLayer = this;

		scene->Load( PROJECT_DIR "/scenes/card.miku" );
	}

	EditorLayer* EditorLayer::GetEditorLayer()
	{
		return s_EditorLayer;
	}

	void EditorLayer::Update( double dt )
	{
		if ( m_EditorLevelStuff.m_CurrentPlayModeState == PlayModeState::PLAYING ) m_Scene->Update( dt );

		if ( m_IsViewportPanelFocused ) m_EditorCamera.Translate( dt );
	}

	void EditorLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		auto& sceneFBO = MikuEngine::Application::GetApplication()->GetSceneFBO();

		sceneFBO.Bind();

		MikuEngine::SpriteRendererSystem::ClearColor( { 0.0f, 0.3f, 0.3f, 1.0f } );

		MikuEngine::CameraData cameraData = { m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjMatrix() };
		m_Scene->RenderInEditor( appLevelStuff, cameraData );

		sceneFBO.UnBind();
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
			m_IsViewportPanelFocused = MikuEditor::ViewportPanel::RenderViewportPanel( *m_Scene );

			ManageTextureSelectionWindows( appLevelStuff );
			ManageShaderSelectionWindows( appLevelStuff );

			m_Scene->RenderImGui( appLevelStuff );

			if ( m_EditorLevelStuff.IsEditorCameraEditorWindowOpen ) m_EditorCamera.RenderImGui( m_EditorLevelStuff );
		}
	}

	void EditorLayer::ManageTextureSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		std::vector<unsigned int> completedTextureWindows;
		completedTextureWindows.reserve( m_TextureSelectionWindow.size() );

		for ( size_t x = 0; x < m_TextureSelectionWindow.size(); x++ )
		{
			auto response = m_TextureSelectionWindow.at( x ).RenderTextureSelectionWindow( appLevelStuff, *m_Scene );

			if ( response == WindowResponse::ERROR || response == WindowResponse::COMPLETED || response == WindowResponse::CLOSED )
			{
				completedTextureWindows.push_back( x );
			}
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedTextureWindows.size(); x++ )
			m_TextureSelectionWindow.erase( m_TextureSelectionWindow.begin() + completedTextureWindows.at( x ) );
	}

	void EditorLayer::ManageShaderSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		std::vector<unsigned int> completedShaderWindow;
		completedShaderWindow.reserve( m_ShaderSelectionWindow.size() );

		for ( size_t x = 0; x < m_ShaderSelectionWindow.size(); x++ )
		{
			auto response = m_ShaderSelectionWindow.at( x ).RenderShaderSelectionWindow( appLevelStuff, *m_Scene );
			if ( response == WindowResponse::ERROR || response == WindowResponse::COMPLETED || response == WindowResponse::CLOSED ) completedShaderWindow.push_back( x );
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedShaderWindow.size(); x++ )
			m_ShaderSelectionWindow.erase( m_ShaderSelectionWindow.begin() + completedShaderWindow.at( x ) );
	}
}
