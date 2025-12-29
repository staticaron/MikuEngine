#include "Panels/MenuBar.h"

#include "MikuEngine/Entity.h"
#include "MikuEngine/Managers/MetaFileManager.h"

namespace MikuEditor
{
	void MenuBar::RenderMenuBar( MikuEngine::Scene& scene )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
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
