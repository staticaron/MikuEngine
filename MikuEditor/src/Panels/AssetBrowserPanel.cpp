#include "Panels/AssetBrowserPanel.h"

#include <filesystem>

#include "imgui.h"

#include "Application.h"
#include "Logger.h"
#include "Scene/Scene.h"

namespace MikuEditor
{
	void AssetBrowserPanel::Init()
	{
		MikuEngine::Texture texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_folder.png" );
		m_IconTextures[ MikuEngine::AssetType::NONE ] = texture;
		MIKU_CLIENT_INFO( "Folder Texture Loaded Into : {}", texture.GetRendererID() );

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_file.png" );
		m_IconTextures[ MikuEngine::AssetType::TEXTURE ] = texture;
		MIKU_CLIENT_INFO( "File Texture Loaded Into : {}", texture.GetRendererID() );
	}

	void AssetBrowserPanel::RenderAssetBrowserPanel( MikuEngine::Scene& scene )
	{
		const auto& appLevelStuff = MikuEngine::Application::GetAppLevelStuff();

		ImGui::ShowDemoWindow();

		ImGui::Begin( "Content Browser" );

		auto canvasSize = ImGui::GetContentRegionAvail();
		auto columns = static_cast<unsigned int>( canvasSize.x / m_IconSize );

		if ( m_ContentBrowserLocation.string() != PROJECT_DIR )
		{
			if ( ImGui::Button( "../" ) ) m_ContentBrowserLocation = m_ContentBrowserLocation.parent_path();
			ImGui::SameLine();
		}

		ImGui::SliderInt( "Icon Size", &m_IconSize, 16, 200 );

		ImGui::BeginTable( "Asset", columns, ImGuiTableFlags_Borders );

		ImGui::PushStyleColor( ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f } );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, { 0.f, 0.f, 0.f, 0.f } );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, { 0.5f, 0.5f, 0.5f, 0.25f } );

		for ( auto item : std::filesystem::directory_iterator( m_ContentBrowserLocation ) )
		{
			auto path = std::filesystem::relative( item.path(), m_RootAssetLocation );

			if ( item.is_directory() )
			{
				if ( ImGui::ImageButton( item.path().c_str(), m_IconTextures.at( MikuEngine::AssetType::NONE ).GetRendererID(), { static_cast<float>( m_IconSize ), static_cast<float>( m_IconSize ) }, { 0, 1 }, { 1, 0 } ) ) m_ContentBrowserLocation = item.path();
				ImGui::Text( "%s", path.c_str() );
				ImGui::TableNextColumn();
			}
			else
			{
				if ( path.extension() == ".meta" ) continue;

				if ( ImGui::ImageButton( item.path().c_str(), m_IconTextures.at( MikuEngine::AssetType::TEXTURE ).GetRendererID(), { static_cast<float>( m_IconSize ), static_cast<float>( m_IconSize ) }, { 0, 1 }, { 1, 0 } ) )
				{
					auto assetType = appLevelStuff.GetAssetPoolManager().GetAssetTypeFromFileExtension( path.extension() );

					auto assetUUID = 0;

					switch ( assetType )
					{
					case MikuEngine::AssetType::TEXTURE:
						assetUUID = appLevelStuff.GetAssetPoolManager().GetTextureManager().GetTextureByFilePath( item.path().string() ).value().GetUUID();
					default:
						assetUUID = 0;
					}

					scene.SetSelectedItem( assetUUID, MikuEngine::SelectableType::ASSET );
				}

				if ( ImGui::BeginDragDropSource() )
				{
					ImGui::SetDragDropPayload( "FILE_DRAG_DROP_PAYLOAD", item.path().string().c_str(), strlen( item.path().string().c_str() ) );
					ImGui::EndDragDropSource();
				}

				ImGui::Text( "%s", path.filename().c_str() );
				ImGui::TableNextColumn();
			}
		}

		ImGui::PopStyleColor( 3 );

		ImGui::EndTable();

		ImGui::End();
	}
}
