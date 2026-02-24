#include "Panels/AssetBrowserPanel.h"

#include <filesystem>

#include "imgui.h"

#include "Logger.h"

namespace MikuEditor
{
	void AssetBrowserPanel::Init()
	{
		MikuEngine::Texture texture;
		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_folder.png" );

		m_IconTextures[ MikuEngine::AssetType::NONE ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_file.png" );
		m_IconTextures[ MikuEngine::AssetType::TEXTURE ] = texture;
	}

	void AssetBrowserPanel::RenderAssetBrowserPanel( MikuEngine::Scene& scene )
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin( "Content Browser" );

		auto canvasSize = ImGui::GetContentRegionAvail();
		auto columns = static_cast<unsigned int>( canvasSize.x / AssetBrowserPanel::m_IconSize.x );

		if ( AssetBrowserPanel::m_ContentBrowserLocation.string() != PROJECT_DIR )
			if ( ImGui::Button( "../" ) ) AssetBrowserPanel::m_ContentBrowserLocation = AssetBrowserPanel::m_ContentBrowserLocation.parent_path();

		auto directories = std::filesystem::directory_iterator( AssetBrowserPanel::m_ContentBrowserLocation );

		// ImGui::PushStyleVar( ImGuiStyleVar_CellPadding, ImVec2( 10.0f, 10.0f ) );

		ImGui::BeginTable( "Texture Button Grid", columns, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit );

		for ( int x = 0; x < columns; x++ )
		{
			auto item = std::next( directories, x );

			auto path = std::filesystem::relative( item->path(), AssetBrowserPanel::m_ContentBrowserLocation );

			if ( item->is_directory() )
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

		ImGui::EndTable();

		// ImGui::PopStyleVar();

		ImGui::End();
	}
}

