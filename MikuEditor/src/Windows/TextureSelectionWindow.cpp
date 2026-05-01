#include "Windows/TextureSelectionWindow.h"

#include "MikuEngine/AppLevelStuff.h"

namespace MikuEditor
{
	TextureSelectionWindow::TextureSelectionWindow( std::function<void( MikuEngine::UUID selectedTextureItem )> onTextureSelection ) : m_OnTextureSelection( onTextureSelection ) {}

	WindowResponse TextureSelectionWindow::RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::Begin( "Select Texture", &m_IsOpen );

		if ( ImGui::BeginTabBar( "Textures" ) )
		{
			if ( ImGui::BeginTabItem( "Project" ) )
			{
				auto canvasSize = ImGui::GetContentRegionAvail();
				auto columns = static_cast<unsigned int>( canvasSize.x / m_ImageSize );

				if ( columns >= 1 )
				{
					// Render each texture as image button in table
					ImGui::BeginTable( "Texture Button Grid", columns );

					auto allTextures = appLevelStuff.GetAssetPoolManager().GetTextureManager().GetAllLoadedTextures();

					for ( auto [ uuid, textureContainer ] : allTextures )
					{
						ImGui::TableNextColumn();
						ImGui::PushID( textureContainer.texture.GetUUID() );

						if ( ImGui::ImageButton( "##TextureBtn", ( void* )( intptr_t )textureContainer.texture.GetRendererID(), ImVec2( 100, 100 ), ImVec2( 1, 1 ), ImVec2( 0, 0 ) ) )
						{
							m_OnTextureSelection( uuid );
							response = WindowResponse::COMPLETED;
						}

						ImGui::PopID();
					}

					ImGui::EndTable();
				}

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem( "Default" ) )
			{
				auto canvasSize = ImGui::GetContentRegionAvail();
				auto columns = static_cast<unsigned int>( canvasSize.x / m_ImageSize );

				// Render each texture as image button in table
				ImGui::BeginTable( "Texture Button Grid", columns );

				auto allTextures = appLevelStuff.GetAssetPoolManager().GetTextureManager().GetAllDefaultTextures();

				for ( auto [ uuid, textureContainer ] : allTextures )
				{
					ImGui::TableNextColumn();
					ImGui::PushID( textureContainer.texture.GetUUID() );
					if ( ImGui::ImageButton( "##TextureBtn", ( void* )( intptr_t )textureContainer.texture.GetRendererID(), ImVec2( 100, 100 ), ImVec2( 1, 1 ), ImVec2( 0, 0 ) ) )
					{
						m_OnTextureSelection( uuid );
						response = WindowResponse::COMPLETED;
					}

					ImGui::PopID();
				}

				ImGui::EndTable();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();

		if ( m_IsOpen == false ) response = WindowResponse::CLOSED;

		return response;
	}
}
