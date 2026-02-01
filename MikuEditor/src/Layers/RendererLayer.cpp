#include "Layers/RendererLayer.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Application.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Entity.h"

namespace MikuEditor
{
	void RendererLayer::Update( double dt )
	{
		m_Scene->Update( dt );
	}

	void RendererLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		auto& gameFBO = MikuEngine::Application::GetApplication()->GetGameFBO();

		{ // Resize the game frame buffer according to the resolution set in main camera
			auto mainCam = m_Scene->GetMainCamera();

			if ( !mainCam.has_value() ) return;

			auto mainCamEntity = mainCam->first;
			auto mainCamComponent = mainCam->second;

			// Resize the game frame buffer according to the resolution set in main camera
			if ( MikuEngine::Application::GetDataContainer().GetGameResolution() != mainCamComponent.m_Resolution ) gameFBO.ResizeBufferTexture( { mainCamComponent.m_Resolution } );
		}

		gameFBO.Bind();

		glClearColor( 0.0f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		m_Scene->Render( appLevelStuff );

		gameFBO.UnBind();
	}

	void RendererLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		RenderFrameBuffer( appLevelStuff );
	}

	void RendererLayer::RenderFrameBuffer( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		ImGui::Begin( "Game" );

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

		ImGui::End();
	}
}
