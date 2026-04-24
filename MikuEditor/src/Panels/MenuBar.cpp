#include "Panels/MenuBar.h"

#include "imgui.h"

#include "Application.h"
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

					MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().StartPlayMode();
				}

				if ( ImGui::MenuItem( "Stop" ) )
				{
					MIKU_CLIENT_INFO( "STOP" );
					editorLevelStuff.SetPlayModeState( PlayModeState::NOTPLAYING );

					MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetSceneManager().EndPlayMode();
				}

				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu( "Scene" ) )
			{
				if ( ImGui::MenuItem( "Save...", "Ctrl+S" ) ) scene.Save();

				if ( ImGui::MenuItem( "Open...", "Ctrl+O" ) ) scene.Load();

				if ( ImGui::MenuItem( "Clean", "Ctrl+W" ) ) scene.Clean();
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

			if ( ImGui::BeginMenu( "Editor" ) )
			{
				if ( ImGui::MenuItem( "Editor Camera Settings" ) ) editorLevelStuff.IsEditorCameraEditorWindowOpen = true;

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}
