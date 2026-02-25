#include "Panels/AssetBrowserPanel.h"

#include <filesystem>

#include "imgui.h"

namespace MikuEditor
{
	void AssetBrowserPanel::RenderAssetBrowserPanel( MikuEngine::Scene& scene )
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin( "Content Browser" );

		if ( AssetBrowserPanel::m_ContentBrowserLocation.string() != PROJECT_DIR )
			if ( ImGui::Button( "../" ) ) AssetBrowserPanel::m_ContentBrowserLocation = AssetBrowserPanel::m_ContentBrowserLocation.parent_path();

		for ( auto item : std::filesystem::directory_iterator( AssetBrowserPanel::m_ContentBrowserLocation ) )
		{
			auto path = std::filesystem::relative( item.path(), AssetBrowserPanel::m_ContentBrowserLocation );

			if ( item.is_directory() )
			{
				if ( ImGui::Button( item.path().string().c_str() ) ) AssetBrowserPanel::m_ContentBrowserLocation = item.path();
			}
			else
			{
				if ( item.path().extension() == ".meta" ) continue;
				ImGui::Button( path.filename().c_str() );

				if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
				{
					ImGui::SetDragDropPayload( "FILE_DRAG_DROP_PAYLOAD", path.c_str(), strlen( path.c_str() ) );
					ImGui::Text( "Moving File: %s", path.filename().c_str() );

					ImGui::EndDragDropSource();
				}
			}
		}

		ImGui::End();
	}
}
