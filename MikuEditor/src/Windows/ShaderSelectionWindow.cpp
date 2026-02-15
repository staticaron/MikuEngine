#include "Windows/ShaderSelectionWindow.h"

#include "imgui.h"

namespace MikuEditor
{
	ShaderSelectionWindow::ShaderSelectionWindow( MikuEngine::UUID uuid ) : m_EntityUUID( uuid ) {}

	WindowResponse ShaderSelectionWindow::RenderShaderSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::Begin( "Select Shader", &m_IsOpen );

		ImGui::End();

		return response;
	}
}
