#include "Layers/RendererLayer.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Application.h"

namespace MikuEditor
{
	void RendererLayer::Update( double dt ) {}
	void RendererLayer::Render( MikuEngine::AppLevelStuff& appLevelStuff ) const
	{
		m_Scene->Render( appLevelStuff );
	}

	void RendererLayer::RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff )
	{
		auto& frameBuffer = MikuEngine::Application::GetApplication()->GetFrameBuffer();
		RenderFrameBuffer( frameBuffer );
	}

	void RendererLayer::RenderFrameBuffer( MikuEngine::FrameBuffer& frameBuffer )
	{
		ImGui::Begin( "Game" );

		ImVec2 windowSize = ImGui::GetWindowViewport()->WorkSize;

		glm::vec2 viewPortSize = MikuEngine::Application::GetDataContainer().GetGameResolution();

		if ( windowSize.x != viewPortSize.x || windowSize.y != viewPortSize.y )
		{
			frameBuffer.ResizeFrameBufferTexture( { windowSize.x, windowSize.y } );
		}

		ImGui::Image( ( void* )( intptr_t )frameBuffer.GetTextureID(), { windowSize.x, windowSize.y }, { 0, 1 }, { 1, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } );
		ImGui::End();
	}
}
