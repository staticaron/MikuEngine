#include "Layers/EditorLayer.h"

#include "Application.h"
#include "Components.h"
#include "Data/CameraData.h"
#include "MikuEngine/Entity.h"
#include "Panels/Panels.h"
#include "Windows/WindowResponse.h"

namespace MikuEditor
{
	static EditorLayer* s_EditorLayer;

	EditorLayer::EditorLayer() : MikuEngine::Layer()
	{
		s_EditorLayer = this;
		m_AssetBrowserPanel.Init();

		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();
		activeScene.Load( PROJECT_DIR "/scenes/card.miku" );
	}

	EditorLayer* EditorLayer::GetEditorLayer()
	{
		return s_EditorLayer;
	}

	void EditorLayer::Update( double dt )
	{
		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();

		// PERFORM THE QUEUED DELETIONS
		activeScene.PerformDeletions();

		if ( m_EditorLevelStuff.m_CurrentPlayModeState == PlayModeState::PLAYING ) return;

		// MOVE EDITOR CAMERA
		//
		//
		//
		//
		if ( m_IsViewportPanelFocused ) m_EditorCamera.Update( dt );

		// TODO: IMPROVE THIS
		// Update the Projection Matrix every frame to account for the changes in the viewport panel size
		//
		//
		//
		//
		m_EditorCamera.UpdateProjectionMatrix();

		// UNIFORM BUFFERS ARE UPDATED!
		//
		//
		//
		//
		MikuEngine::Application::GetAppLevelStuff().GetRenderer().GetUniformBufferManager().UpdateEditorCameraData( {
		    m_EditorCamera.GetProjMatrix(), m_EditorCamera.GetViewMatrix(), {	      m_EditorCamera.GetPosition(), 0.0f},
			    {m_EditorCamera.GetForwardDirection(), 0.0f}
		   } );

		const auto& mainLight = activeScene.GetMainLight();
		if ( mainLight.has_value() == false )
			MikuEngine::Application::GetAppLevelStuff().GetRenderer().GetUniformBufferManager().UpdateLightingData( {} );
		else
		{
			const auto& mainLightTransform = mainLight.value().first.GetReadOnlyComponent<MikuEngine::TransformComponent>();

			MikuEngine::Application::GetAppLevelStuff().GetRenderer().GetUniformBufferManager().UpdateLightingData( {
			    {    mainLightTransform.Position, 0.0f},
				   {mainLightTransform.GetForward(), 0.0f},
			      { mainLight.value().second.Color, 0.0f},
				mainLight.value().second.Intensity, mainLight.value().second.AmbientIntensity, mainLight.value().second.SpecularStrength
			 } );
		}

		// PANELS ARE UPDATED
		//
		//
		//
		//
		m_ViewportPanel.Update();
	}

	void EditorLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		if ( m_ViewportPanel.GetWindowVisible() == false ) return;

		// Editor Camera Data to be sent for rendering
		MikuEngine::CameraData cameraData = { m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjMatrix() };

		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();
		activeScene.RenderInEditor( appLevelStuff, cameraData );
	}

	void EditorLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();

		if ( m_EditorLevelStuff.m_CurrentPlayModeState == PlayModeState::PLAYING )
		{
			MikuEditor::MenuBar::RenderMenuBar( *this, appLevelStuff, m_EditorLevelStuff, activeScene );
			MikuEditor::EditorOverlayPanel::RenderEditorOverlayPanel( *this, appLevelStuff, m_EditorLevelStuff, activeScene );
		}
		else
		{
			MikuEditor::MenuBar::RenderMenuBar( *this, appLevelStuff, m_EditorLevelStuff, activeScene );
			MikuEditor::HierarchyPanel::RenderHierarchy( activeScene );
			MikuEditor::InspectorPanel::RenderInspectorPanel( *this, appLevelStuff, activeScene );
			m_AssetBrowserPanel.RenderAssetBrowserPanel( activeScene );
			MikuEditor::EditorOverlayPanel::RenderEditorOverlayPanel( *this, appLevelStuff, m_EditorLevelStuff, activeScene );
			m_IsViewportPanelFocused = m_ViewportPanel.RenderViewportPanel( *this, activeScene );

			ManageTextureSelectionWindows( appLevelStuff );
			ManageShaderSelectionWindows( appLevelStuff );
			ManageMaterialSelectionWindows( appLevelStuff );
			ManageModelSelectionWindows( appLevelStuff );

			activeScene.RenderImGui( appLevelStuff );

			if ( m_EditorLevelStuff.IsEditorCameraEditorWindowOpen ) m_EditorCamera.RenderImGui( m_EditorLevelStuff );
		}
	}

	void EditorLayer::ManageTextureSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();

		std::vector<unsigned int> completedTextureWindows;
		completedTextureWindows.reserve( m_TextureSelectionWindow.size() );

		for ( size_t x = 0; x < m_TextureSelectionWindow.size(); x++ )
		{
			auto response = m_TextureSelectionWindow.at( x ).RenderTextureSelectionWindow( std::to_string( x ), appLevelStuff, activeScene );

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
		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();

		std::vector<unsigned int> completedShaderWindow;
		completedShaderWindow.reserve( m_ShaderSelectionWindow.size() );

		for ( size_t x = 0; x < m_ShaderSelectionWindow.size(); x++ )
		{
			auto response = m_ShaderSelectionWindow.at( x ).RenderShaderSelectionWindow( std::to_string( x ), appLevelStuff, activeScene );
			if ( response == WindowResponse::ERROR || response == WindowResponse::COMPLETED || response == WindowResponse::CLOSED ) completedShaderWindow.push_back( x );
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedShaderWindow.size(); x++ )
			m_ShaderSelectionWindow.erase( m_ShaderSelectionWindow.begin() + completedShaderWindow.at( x ) );
	}

	void EditorLayer::ManageMaterialSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();

		std::vector<unsigned int> completedMaterialWindow;
		completedMaterialWindow.reserve( m_MaterialSelectionWindow.size() );

		for ( size_t x = 0; x < m_MaterialSelectionWindow.size(); x++ )
		{
			auto response = m_MaterialSelectionWindow.at( x ).RenderMaterialSelectionWindow( std::to_string( x ), appLevelStuff, activeScene );
			if ( response == WindowResponse::ERROR || response == WindowResponse::COMPLETED || response == WindowResponse::CLOSED ) completedMaterialWindow.push_back( x );
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedMaterialWindow.size(); x++ )
			m_MaterialSelectionWindow.erase( m_MaterialSelectionWindow.begin() + completedMaterialWindow.at( x ) );
	}

	void EditorLayer::ManageModelSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		auto& activeScene = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().GetScene();

		std::vector<unsigned int> completedModelWindow;
		completedModelWindow.reserve( m_ModelSelectionWindow.size() );

		for ( size_t x = 0; x < m_ModelSelectionWindow.size(); x++ )
		{
			auto response = m_ModelSelectionWindow.at( x ).RenderModelSelectionWindow( std::to_string( x ), appLevelStuff, activeScene );
			if ( response == WindowResponse::ERROR || response == WindowResponse::COMPLETED || response == WindowResponse::CLOSED ) completedModelWindow.push_back( x );
		}

		// Remove the texture selection windows that are completed!
		for ( size_t x = 0; x < completedModelWindow.size(); x++ )
			m_ModelSelectionWindow.erase( m_ModelSelectionWindow.begin() + completedModelWindow.at( x ) );
	}
}
