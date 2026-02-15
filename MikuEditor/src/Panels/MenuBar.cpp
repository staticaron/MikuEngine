#include "Panels/MenuBar.h"

#include "Layers/EditorLayer.h"
#include "MikuEngine/Logger.h"
#include "MikuEngine/Managers/MetaFileManager.h"
#include "MikuEngine/ScriptRegistry.h"

namespace MikuEditor
{
	void MenuBar::RenderMenuBar( EditorLayer& editorLayer, const MikuEngine::AppLevelStuff& appLevelStuff, EditorLevelStuff& editorLevelStuff, MikuEngine::Scene& scene )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "File" ) )
			{
				if ( ImGui::MenuItem( "Play" ) )
				{
					MIKU_CLIENT_INFO( "PLAY" );
					editorLevelStuff.SetPlayModeState( PlayModeState::PLAYING );
				}

				if ( ImGui::MenuItem( "Stop" ) )
				{
					MIKU_CLIENT_INFO( "STOP" );
					editorLevelStuff.SetPlayModeState( PlayModeState::NOTPLAYING );
				}

				ImGui::EndMenu();
			}
			if ( ImGui::BeginMenu( "Scene" ) )
			{
				ImGui::Separator();
				if ( ImGui::MenuItem( "Save...", "CTRL+S" ) ) scene.Save();
				ImGui::Separator();
				if ( ImGui::MenuItem( "Open...", "CTRL+O" ) ) scene.Load();
				ImGui::Separator();
				if ( ImGui::MenuItem( "Clean", "CTRL+W" ) ) scene.Clean();
				ImGui::EndMenu();
			}
			if ( ImGui::BeginMenu( "Assets" ) )
			{
				ImGui::Separator();

				if ( ImGui::MenuItem( "Refresh UUIDs" ) ) MikuEngine::MetaFileManager::RefreshMetaFiles();
				if ( ImGui::MenuItem( "Refresh Scripts" ) )
				{
					MikuEngine::ScriptRegistry::RefreshScripts();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}
