#include "Layers/RendererLayer.h"

#include "imgui.h"
#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Application.h"

namespace MikuEditor
{
	void RendererLayer::Update( double dt ) {}
	void RendererLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		auto& gameFBO = MikuEngine::Application::GetApplication()->GetGameFBO();

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
#define ON 1
#if ON
		ImGui::Begin( "Game" );

		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		glm::vec2 viewPortSize = MikuEngine::Application::GetDataContainer().GetGameResolution();

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
#else
		glm::vec2 viewPortSize2 = MikuEngine::Application::GetDataContainer().GetViewportSize();

		ImGui::Begin( "Game" );

		ImVec2 windowSize2 = ImGui::GetContentRegionAvail();

		auto& frameBuffer2 = MikuEngine::Application::GetApplication()->GetFrameBuffer();

		if ( windowSize2.x != viewPortSize2.x || windowSize2.y != viewPortSize2.y )
		{
			frameBuffer2.ResizeFrameBufferTexture( { windowSize2.x, windowSize2.y } );
		}

		ImGui::Image( ( void* )( intptr_t )frameBuffer2.GetTextureID(), { windowSize2.x, windowSize2.y }, { 0, 1 }, { 1, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } );

		ImGui::End();
#endif
	}
}
