#include "Panels/AssetBrowserPanel.h"

#include <filesystem>

#include "imgui.h"

#include "Application.h"
#include "Logger.h"
#include "MikuEngine/Data/AssetType.h"
#include "Scene/Scene.h"
#include "UUID.h"

namespace MikuEditor
{
	void AssetBrowserPanel::Init()
	{
		MikuEngine::Texture texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_folder.png" );
		m_IconTextures[ MikuEngine::AssetType::NONE ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_file.png" );
		m_IconTextures[ MikuEngine::AssetType::FILE ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_model.png" );
		m_IconTextures[ MikuEngine::AssetType::MODEL ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_shader.png" );
		m_IconTextures[ MikuEngine::AssetType::SHADER ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_script.png" );
		m_IconTextures[ MikuEngine::AssetType::SCRIPT ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_texture.png" );
		m_IconTextures[ MikuEngine::AssetType::TEXTURE ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_scene.png" );
		m_IconTextures[ MikuEngine::AssetType::SCENE ] = texture;

		texture.LoadFromFile( RESOURCE_DIR "/icons/asset_material.png" );
		m_IconTextures[ MikuEngine::AssetType::MATERIAL ] = texture;
	}

	void AssetBrowserPanel::RenderAssetBrowserPanel( MikuEngine::Scene& scene )
	{
		const auto& appLevelStuff = MikuEngine::Application::GetAppLevelStuff();

		ImGui::ShowDemoWindow();

		ImGui::Begin( "Content Browser" );

		auto canvasSize = ImGui::GetContentRegionAvail();
		auto columns = static_cast<unsigned int>( canvasSize.x / ( m_IconSize + ImGui::GetStyle().ItemSpacing.x * 2 ) );
		if ( columns < 1 ) columns = 1;

		// Render the back button
		if ( m_ContentBrowserLocation.string() != PROJECT_DIR )
		{
			if ( ImGui::Button( "../" ) ) m_ContentBrowserLocation = m_ContentBrowserLocation.parent_path();
			ImGui::SameLine();
		}

		// Render the icon size adjust slider
		ImGui::SliderInt( "Icon Size", &m_IconSize, 16, 200 );

		ImGui::PushStyleColor( ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f } );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, { 0.f, 0.f, 0.f, 0.f } );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, { 0.5f, 0.5f, 0.5f, 0.25f } );

		ImGui::Columns( columns, NULL, false );

		for ( auto item : std::filesystem::directory_iterator( m_ContentBrowserLocation ) )
		{
			auto relativePath = std::filesystem::relative( item.path(), m_RootAssetLocation );

			if ( item.is_directory() )
				RenderFolderIcon( item.path() );
			else
			{
				if ( relativePath.extension() == ".meta" ) continue;

				std::function<void( MikuEngine::UUID assetUUID, MikuEngine::AssetType )> onClickFunc = [ &scene, &item, &appLevelStuff ]( MikuEngine::UUID assetUUID, MikuEngine::AssetType assetType ) {
					// comment
					scene.SetSelectedItem( assetUUID, MikuEngine::SelectableType::ASSET, assetType );
				};

				RenderFileIcon( item.path(), onClickFunc );
			}

			ImGui::NextColumn();
		}

		ImGui::Columns( 1 );

		ImGui::PopStyleColor( 3 );

		if ( ImGui::BeginPopupContextWindow() )
		{
			if ( ImGui::MenuItem( "Create Material" ) ) MikuEngine::Material::CreateAssetAtPath( "gigaNewMat", m_ContentBrowserLocation );
			if ( ImGui::MenuItem( "Create Shader" ) ) MikuEngine::Shader::CreateAssetAtPath( "gigaNewShader", m_ContentBrowserLocation );

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void AssetBrowserPanel::RenderFolderIcon( const std::filesystem::path& folderPath )
	{
		auto relativePath = std::filesystem::relative( folderPath, m_RootAssetLocation );

		if ( ImGui::ImageButton( folderPath.c_str(), m_IconTextures.at( MikuEngine::AssetType::NONE ).GetRendererID(), { static_cast<float>( m_IconSize ), static_cast<float>( m_IconSize ) }, { 0, 1 }, { 1, 0 } ) ) m_ContentBrowserLocation = folderPath;
		ImGui::Text( "%s", relativePath.c_str() );
	}

	void AssetBrowserPanel::RenderFileIcon( const std::filesystem::path& filePath, std::function<void( MikuEngine::UUID, MikuEngine::AssetType )> onClickFunc )
	{
		auto& appLevelStuff = MikuEngine::Application::GetAppLevelStuff();
		auto relativePath = std::filesystem::relative( filePath, m_RootAssetLocation );
		auto assetType = appLevelStuff.GetAssetPoolManager().GetAssetTypeFromFileExtension( relativePath.extension() );

		auto iconTexture = m_IconTextures.at( assetType );

		if ( ImGui::ImageButton( filePath.c_str(), iconTexture.GetRendererID(), { static_cast<float>( m_IconSize ), static_cast<float>( m_IconSize ) }, { 0, 1 }, { 1, 0 } ) )
		{
			std::optional<MikuEngine::UUID> assetUUID = std::nullopt;

			switch ( assetType )
			{
			case MikuEngine::AssetType::TEXTURE: {
				assetUUID = appLevelStuff.GetAssetPoolManager().GetTextureManager().GetTextureByFilePath( filePath.string() ).value()->texture.GetUUID();
				break;
			}
			case MikuEngine::AssetType::MATERIAL: {
				assetUUID = appLevelStuff.GetAssetPoolManager().GetMaterialManager().GetMaterialByFilePath( filePath.string() ).value()->GetUUID();
				break;
			}
			case MikuEngine::AssetType::MODEL: {
				assetUUID = appLevelStuff.GetAssetPoolManager().GetModelManager().GetModelByFilePath( filePath.string() ).index.uuid;
				break;
			}
			case MikuEngine::AssetType::SCENE:
				break;
			case MikuEngine::AssetType::SHADER: {
				assetUUID = appLevelStuff.GetAssetPoolManager().GetShaderManager().GetShaderByFilePath( filePath.string() ).index.uuid;
				break;
			}
			default:
				break;
			}

			if ( !assetUUID.has_value() )
			{
				MIKU_CLIENT_WARN( "Button Click Behaviour Not Implemented!" );
				return;
			};

			onClickFunc( assetUUID.value(), assetType );
		}

		if ( ImGui::BeginDragDropSource() )
		{
			switch ( assetType )
			{
			case MikuEngine::AssetType::TEXTURE: {
				ImGui::SetDragDropPayload( "TEXTURE_DRAG_DROP_PAYLOAD", filePath.string().c_str(), strlen( filePath.string().c_str() ) + 1 );
				break;
			}
			case MikuEngine::AssetType::MODEL: {
				ImGui::SetDragDropPayload( "MODEL_DRAG_DROP_PAYLOAD", filePath.string().c_str(), strlen( filePath.string().c_str() ) + 1 );
				break;
			}
			case MikuEngine::AssetType::MATERIAL: {
				ImGui::SetDragDropPayload( "MATERIAL_DRAG_DROP_PAYLOAD", filePath.string().c_str(), strlen( filePath.string().c_str() ) + 1 );
				break;
			}
			case MikuEngine::AssetType::SHADER: {
				ImGui::SetDragDropPayload( "SHADER_DRAG_DROP_PAYLOAD", filePath.string().c_str(), strlen( filePath.string().c_str() ) + 1 );
				break;
			}
			default: {
				ImGui::SetDragDropPayload( "FILE_DRAG_DROP_PAYLOAD", filePath.string().c_str(), strlen( filePath.string().c_str() ) + 1 );
				break;
			}
			}

			ImGui::Text( "%s", filePath.filename().c_str() );

			ImGui::EndDragDropSource();
		}

		ImGui::Text( "%s", relativePath.filename().c_str() );
	}
}
