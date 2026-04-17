#include "Layers/RendererLayer.h"

#include "Components/CameraComponent.h"
#include "Components/DirectionLightComponent.h"
#include "Components/TransformComponent.h"
#include "Layers/EditorLayer.h"
#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Application.h"
#include "MikuEngine/ScriptRegistry.h"
#include "MikuEngine/Systems.h"

namespace MikuEditor
{
	RendererLayer::RendererLayer( MikuEngine::Scene* scene ) : MikuEngine::Layer( scene )
	{
		MikuEngine::ScriptRegistry::RefreshScripts();
	}

	void RendererLayer::Update( double dt )
	{
		auto mainCamera = m_Scene->GetMainCamera();

		if ( mainCamera.has_value() )
		{
			auto mainCameraEntity = mainCamera->first;
			auto mainCameraComponent = mainCamera->second;

			MikuEngine::Application::GetAppLevelStuff().GetRenderer().GetUniformBufferManager().UpdateGameMatrixData( { MikuEngine::CameraSystem::GetProjMatrix( mainCameraComponent ), MikuEngine::CameraSystem::GetViewMatrix( mainCameraEntity ) } );

			auto mainLight = m_Scene->GetMainLight();

			if ( mainLight.has_value() )
			{
				auto mainLightEntity = mainLight->first;
				auto mainLightComponent = mainLight->second;
				auto mainLightTransform = mainLightEntity.GetComponent<MikuEngine::TransformComponent>();

				MikuEngine::Application::GetAppLevelStuff().GetRenderer().GetUniformBufferManager().UpdateLightingData( {
				    {    mainLightTransform.Position, 1.0},
					  {mainLightTransform.GetForward(), 1.0},
				    {	      mainLightComponent.Color, 1.0},
				       mainLightComponent.Intensity
				    } );
			}
		}

		if ( EditorLayer::GetEditorLayer()->GetEditorLayerInfo().GetPlayModeState() != PlayModeState::PLAYING ) return;

		m_Scene->Update( dt );
	}

	void RendererLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		m_Scene->Render( appLevelStuff );
	}

	void RendererLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		RenderFrameBuffer( appLevelStuff );
	}

	void RendererLayer::RenderFrameBuffer( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		if ( ImGui::Begin( "Game" ) )
		{
			ImVec2 windowSize = ImGui::GetContentRegionAvail();

			auto& frameBuffer = MikuEngine::Application::GetApplication()->GetGameFBO();

			ImVec2 imageSize;
			ImVec2 imageStartPoint;

			if ( windowSize.x > windowSize.y * frameBuffer.GetAspectRatio() )
			{
				imageSize.x = windowSize.y * frameBuffer.GetAspectRatio();
				imageSize.y = windowSize.y;

				imageStartPoint = { ( windowSize.x - imageSize.x + 20 ) * 0.5f, 0 };
			}
			else
			{
				imageSize.x = windowSize.x;
				imageSize.y = windowSize.x / frameBuffer.GetAspectRatio();

				imageStartPoint = { 0, ( windowSize.y - imageSize.y + 20 ) * 0.5f };
			}

			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos( ImVec2( cursorPos.x + ( windowSize.x - imageSize.x ) * 0.5f, cursorPos.y + ( windowSize.y - imageSize.y ) * 0.5f ) );

			ImGui::Image( ( void* )( intptr_t )frameBuffer.GetTextureID(), { imageSize.x, imageSize.y }, { 0, 1 }, { 1, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } );
		}

		ImGui::End();
	}
}
