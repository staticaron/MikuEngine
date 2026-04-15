#include "Layers/EditorLayer.h"

#include "Application.h"
#include "Components.h"
#include "Data/CameraData.h"
#include "Logger.h"
#include "MikuEngine/Entity.h"
#include "Panels/Panels.h"
#include "Windows/WindowResponse.h"

namespace MikuEditor
{
	static EditorLayer* s_EditorLayer;

	EditorLayer::EditorLayer( MikuEngine::Scene* scene ) : MikuEngine::Layer( scene )
	{
		s_EditorLayer = this;
		m_AssetBrowserPanel.Init();
		scene->Load( PROJECT_DIR "/scenes/card.miku" );
	}

	EditorLayer* EditorLayer::GetEditorLayer()
	{
		return s_EditorLayer;
	}

	void EditorLayer::Update( double dt )
	{
		// Do nothing if playmode is active
		if ( m_EditorLevelStuff.m_CurrentPlayModeState == PlayModeState::PLAYING ) return;

		// handle camera movement when viewport is active
		if ( m_IsViewportPanelFocused ) m_EditorCamera.Update( dt );

		// TODO: IMPROVE THIS
		// Update the Projection Matrix every frame to account for the changes in the viewport panel size
		m_EditorCamera.UpdateProjectionMatrix();

		// Update the UniformBuffers and feed in the new matrices
		MikuEngine::Application::GetAppLevelStuff().GetRenderer().GetUniformBufferManager().UpdateEditorMatrixData( { m_EditorCamera.GetProjMatrix(), m_EditorCamera.GetViewMatrix() } );

		const auto& mainLight = m_Scene->GetMainLight();
		if ( mainLight.has_value() == false )
			MIKU_CLIENT_WARN( "No Active Light" );
		else
		{
			const auto& mainLightTransform = mainLight.value().first.GetReadOnlyComponent<MikuEngine::TransformComponent>();
			MikuEngine::Application::GetAppLevelStuff().GetRenderer().GetUniformBufferManager().UpdateLightingData( { mainLight.value().second.Color, mainLight.value().second.Intensity } );
		}
	}

	void EditorLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		// Editor Camera Data to be sent for rendering
		MikuEngine::CameraData cameraData = { m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjMatrix() };
		m_Scene->RenderInEditor( appLevelStuff, cameraData );
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
			m_AssetBrowserPanel.RenderAssetBrowserPanel( *m_Scene );
			MikuEditor::EditorOverlayPanel::RenderEditorOverlayPanel( *this, appLevelStuff, m_EditorLevelStuff, *m_Scene );
			m_IsViewportPanelFocused = MikuEditor::ViewportPanel::RenderViewportPanel( *m_Scene );

			ManageTextureSelectionWindows( appLevelStuff );
			ManageShaderSelectionWindows( appLevelStuff );
			ManageMaterialSelectionWindows( appLevelStuff );
			ManageModelSelectionWindows( appLevelStuff );

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

	void EditorLayer::ManageMaterialSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		std::vector<unsigned int> completedMaterialWindow;
		completedMaterialWindow.reserve( m_MaterialSelectionWindow.size() );

		for ( size_t x = 0; x < m_MaterialSelectionWindow.size(); x++ )
		{
			auto response = m_MaterialSelectionWindow.at( x ).RenderMaterialSelectionWindow( appLevelStuff, *m_Scene );
			if ( response == WindowResponse::ERROR || response == WindowResponse::COMPLETED || response == WindowResponse::CLOSED ) completedMaterialWindow.push_back( x );
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedMaterialWindow.size(); x++ )
			m_MaterialSelectionWindow.erase( m_MaterialSelectionWindow.begin() + completedMaterialWindow.at( x ) );
	}

	void EditorLayer::ManageModelSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		std::vector<unsigned int> completedModelWindow;
		completedModelWindow.reserve( m_ModelSelectionWindow.size() );

		for ( size_t x = 0; x < m_ModelSelectionWindow.size(); x++ )
		{
			auto response = m_ModelSelectionWindow.at( x ).RenderModelSelectionWindow( appLevelStuff, *m_Scene );
			if ( response == WindowResponse::ERROR || response == WindowResponse::COMPLETED || response == WindowResponse::CLOSED ) completedModelWindow.push_back( x );
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedModelWindow.size(); x++ )
			m_ModelSelectionWindow.erase( m_ModelSelectionWindow.begin() + completedModelWindow.at( x ) );
	}
}
