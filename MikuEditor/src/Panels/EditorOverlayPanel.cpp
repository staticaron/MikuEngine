#include "Panels/EditorOverlayPanel.h"

namespace MikuEditor
{
	void EditorOverlayPanel::RenderEditorOverlayPanel( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, EditorLevelStuff& editorLevelStuff, MikuEngine::Scene& scene )
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;

		const unsigned int padding = 10.0f;
		ImVec2 viewportSize = ImGui::GetMainViewport()->WorkSize;
		ImVec2 viewportPos = ImGui::GetMainViewport()->WorkPos;

		ImVec2 overlayPos = { viewportPos.x + viewportSize.x - padding, viewportPos.y + viewportSize.y - padding };
		ImVec2 overlayPivot = { 1.0f, 1.0f };

		ImGui::SetNextWindowPos( overlayPos, 0, overlayPivot );
		ImGui::SetNextWindowBgAlpha( 0.5f );

		ImGui::Begin( "##EditorOverlay", nullptr, flags );

		ImGui::Text( editorLevelStuff.GetPlayModeState() == PlayModeState::PLAYING ? "Play Mode : Playing" : "Play Mode : Not Playing" );
		ImGui::Text( "FPS : %d", appLevelStuff.GetFPS() );

		ImGui::End();
	}

}
