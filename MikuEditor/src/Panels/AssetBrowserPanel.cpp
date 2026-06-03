#include "Panels/AssetBrowserPanel.h"

#include <filesystem>

#include "imgui.h"
#include "imgui_internal.h"

#include "Application.h"
#include "Logger.h"
#include "MikuEngine/Data/AssetType.h"
#include "Scene/Scene.h"
#include "UUID.h"

#include "Layers/EditorLayer.h"

namespace MikuEditor
{
	void AssetBrowserPanel::Init()
	{
		const auto& dataContainer = MikuEngine::Application::GetDataContainer();
		m_RootAssetLocation = dataContainer.GetProjectDir();
		m_ContentBrowserLocation = dataContainer.GetProjectDir();

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

	void AssetBrowserPanel::RenderAssetBrowserPanel( EditorLayer& editorLayer, MikuEngine::Scene& scene )
	{
		const auto& appLevelStuff = MikuEngine::Application::GetAppLevelStuff();
		const auto& dataContainer = MikuEngine::Application::GetDataContainer();

		ImGui::ShowDemoWindow();

		ImGui::Begin( "Content Browser" );

		auto canvasSize = ImGui::GetContentRegionAvail();
		auto columns = static_cast<unsigned int>( canvasSize.x / ( m_IconSize + ImGui::GetStyle().ItemSpacing.x * 2 ) );
		if ( columns < 1 ) columns = 1;

		// Render the back button
		if ( m_ContentBrowserLocation.string() != dataContainer.GetProjectDir() )
		{
			if ( ImGui::Button( "<BACK>" ) ) m_ContentBrowserLocation = m_ContentBrowserLocation.parent_path();
			ImGui::SameLine();
		}

		// Render the icon size adjust slider
		ImGui::SliderInt( "Icon Size", &m_IconSize, 16, 200 );

		// SEGREGATE THE FILES AND FOLDERS FOR SEPARATE RENDERING
		std::vector<std::filesystem::path> folders;
		std::vector<std::filesystem::path> files;

		for ( auto directory_item : std::filesystem::directory_iterator( m_ContentBrowserLocation ) )
		{
			auto relativePath = std::filesystem::relative( directory_item.path(), m_RootAssetLocation );

			if ( directory_item.is_directory() )
				folders.push_back( directory_item.path() );
			else
			{
				if ( relativePath.extension() == ".meta" ) continue;
				files.push_back( directory_item.path() );
			}
		}

		ImGui::PushStyleColor( ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f } );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, { 0.f, 0.f, 0.f, 0.f } );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, { 0.5f, 0.5f, 0.5f, 0.25f } );

		// START THE COLUMNS
		ImGui::Columns( columns, NULL, false );

		// RENDER FOLDERS
		for ( const auto& folder : folders )
		{
			std::function<void()> onClickFunc = [ &editorLayer, &folder ]() {
				// comment
				editorLayer.SetSeletedAsset( MikuEngine::AssetType::FOLDER, MikuEngine::UUID( 0 ), folder );
			};

			RenderFolderIcon( folder, onClickFunc );
			ImGui::NextColumn();
		}

		// RENDER FILES
		for ( const auto& file : files )
		{
			std::function<void( MikuEngine::UUID, MikuEngine::AssetType, const std::filesystem::path& assetPath )> onClickFunc = [ &editorLayer, &file, &appLevelStuff ]( MikuEngine::UUID assetUUID, MikuEngine::AssetType assetType, const std::filesystem::path& assetPath ) {
				// comment
				editorLayer.SetSeletedAsset( assetType, assetUUID, assetPath );
			};

			RenderFileIcon( file, onClickFunc );

			ImGui::NextColumn();
		}

		// RESET THE COLUMNS
		ImGui::Columns( 1 );
		ImGui::PopStyleColor( 3 );

		if ( ImGui::BeginPopupContextWindow() )
		{
			if ( ImGui::MenuItem( "Create Folder" ) ) std::filesystem::create_directory( m_ContentBrowserLocation / std::filesystem::path( "NewFolder" ) );
			ImGui::Separator();
			if ( ImGui::MenuItem( "Create Material" ) ) MikuEngine::MaterialManager::CreateAssetAtPath( "gigaNewMat", m_ContentBrowserLocation );
			if ( ImGui::MenuItem( "Create Shader" ) ) MikuEngine::Shader::CreateAssetAtPath( "gigaNewShader", m_ContentBrowserLocation );

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void AssetBrowserPanel::RenderFolderIcon( const std::filesystem::path& folderPath, std::function<void()> onClickFunc )
	{
		auto relativePath = std::filesystem::relative( folderPath, m_RootAssetLocation );

		ImGuiButtonFlags folderBtnFlags = ImGuiButtonFlags_PressedOnDoubleClick;

		ImGui::ImageButton( folderPath.c_str(), m_IconTextures.at( MikuEngine::AssetType::NONE ).GetRendererID(), { static_cast<float>( m_IconSize ), static_cast<float>( m_IconSize ) }, { 0, 1 }, { 1, 0 } );

		if ( ImGui::IsItemHovered() )
		{
			if ( ImGui::IsMouseClicked( 0 ) )
			{
				onClickFunc();
			}

			if ( ImGui::IsMouseDoubleClicked( 0 ) )
			{
				m_ContentBrowserLocation = folderPath;
			}
		}

		ImGui::Text( "%s", relativePath.stem().c_str() );
	}

	void AssetBrowserPanel::RenderFileIcon( const std::filesystem::path& filePath, std::function<void( MikuEngine::UUID, MikuEngine::AssetType, const std::filesystem::path& )> onClickFunc )
	{
		auto& appLevelStuff = MikuEngine::Application::GetAppLevelStuff();
		auto relativePath = std::filesystem::relative( filePath, m_RootAssetLocation );
		auto assetType = appLevelStuff.GetAssetPoolManager().GetAssetTypeFromFileExtension( relativePath.extension() );

		const MikuEngine::Texture* iconTexture = nullptr;

		if ( assetType == MikuEngine::AssetType::TEXTURE )
		{
			auto textureFetch = appLevelStuff.GetAssetPoolManager().GetTextureManager().GetTextureByFilePath( filePath );
			if ( textureFetch.has_value() )
				iconTexture = &textureFetch.value()->texture;
			else
				iconTexture = &m_IconTextures.at( assetType );
		}
		else
			iconTexture = &m_IconTextures.at( assetType );

		if ( ImGui::ImageButton( filePath.c_str(), iconTexture->GetRendererID(), { static_cast<float>( m_IconSize ), static_cast<float>( m_IconSize ) }, { 0, 1 }, { 1, 0 } ) )
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
				assetUUID = appLevelStuff.GetAssetPoolManager().GetModelManager().GetModelByFilePath( filePath.string() ).value()->index.uuid;
				break;
			}
			case MikuEngine::AssetType::SCENE:
				break;
			case MikuEngine::AssetType::SHADER: {
				assetUUID = appLevelStuff.GetAssetPoolManager().GetShaderManager().GetShaderByFilePath( filePath.string() ).value()->index.uuid;
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

			onClickFunc( assetUUID.value(), assetType, filePath );
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
