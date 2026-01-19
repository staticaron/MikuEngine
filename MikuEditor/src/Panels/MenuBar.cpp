#include "Panels/MenuBar.h"

#include "Layers/EditorLayer.h"
#include "MikuEngine/Logger.h"
#include "MikuEngine/Managers/MetaFileManager.h"

namespace MikuEditor
{
	void MenuBar::RenderMenuBar( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, EditorLevelStuff& editorLevelStuff, MikuEngine::Scene& scene )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "File" ) )
			{
				ImGui::Separator();
				if ( ImGui::MenuItem( "Play" ) )
				{
					MIKU_CLIENT_INFO( "PLAY" );
					editorLevelStuff.m_CurrentPlayModeState = PlayModeState::PLAYING;
				}

				ImGui::Separator();
				if ( ImGui::MenuItem( "Stop" ) )
				{
					MIKU_CLIENT_INFO( "STOP" );
					editorLevelStuff.m_CurrentPlayModeState = PlayModeState::NOTPLAYING;
				}

				ImGui::EndMenu();
			}
			if ( ImGui::BeginMenu( "Scene" ) )
			{
				ImGui::Separator();
				if ( ImGui::MenuItem( "Save...", "CTRL+S" ) ) scene.Save( "main.miku" );
				ImGui::Separator();
				if ( ImGui::MenuItem( "Open...", "CTRL+O" ) ) scene.Load( "main.miku" );
				ImGui::Separator();
				if ( ImGui::MenuItem( "Clean", "CTRL+W" ) ) scene.Clean();
				ImGui::EndMenu();
			}
			if ( ImGui::BeginMenu( "Assets" ) )
			{
				ImGui::Separator();
				if ( ImGui::MenuItem( "Refresh" ) ) MikuEngine::MetaFileManager::RefreshMetaFiles();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}
