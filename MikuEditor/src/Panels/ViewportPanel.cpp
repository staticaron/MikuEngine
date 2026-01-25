#include "Panels/ViewportPanel.h"

#include "MikuEngine/Application.h"

#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	void ViewportPanel::RenderViewportPanel( MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Viewport" );

		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		glm::vec2 viewPortSize = MikuEngine::Application::GetDataContainer().GetViewportSize();

		auto& frameBuffer = MikuEngine::Application::GetApplication()->GetSceneFBO();

		if ( windowSize.x != viewPortSize.x || windowSize.y != viewPortSize.y )
		{
			frameBuffer.ResizeBufferTexture( { windowSize.x, windowSize.y } );
		}

		ImGui::Image( ( void* )( intptr_t )frameBuffer.GetTextureID(), { windowSize.x, windowSize.y }, { 0, 1 }, { 1, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } );

		ImGui::End();
	}
}
