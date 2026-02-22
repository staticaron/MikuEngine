#include "Panels/AssetBrowserPanel.h"

#include <filesystem>

#include "imgui.h"

namespace MikuEditor
{
	void AssetBrowserPanel::RenderAssetBrowserPanel( MikuEngine::Scene& scene )
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin( "Content Browser" );

		for ( auto item : std::filesystem::recursive_directory_iterator( PROJECT_DIR ) )
			ImGui::Text( "%s", item.path().c_str() );

		ImGui::End();
	}
}
