#include "Windows/TextureSelectionWindow.h"

#include "Components.h"
#include "MikuEngine/AppLevelStuff.h"

namespace MikuEditor
{
	TextureSelectionWindow::TextureSelectionWindow( MikuEngine::UUID entityUUID ) : m_EntityUUID( entityUUID ) {}

	WindowResponse TextureSelectionWindow::RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::Begin( "Select Texture", &m_IsOpen );

		auto canvasSize = ImGui::GetContentRegionAvail();
		auto columns = static_cast<unsigned int>( canvasSize.x / m_ImageSize );

		// Render each texture as image button in table
		ImGui::BeginTable( "Texture Button Grid", columns );

		auto allTextures = appLevelStuff.GetAssetPoolManager().GetTextureManager().GetAllLoadedTextures();

		for ( auto [ uuid, texture ] : allTextures )
		{
			ImGui::TableNextColumn();
			ImGui::PushID( texture.GetUUID() );

			if ( ImGui::ImageButton( "##TextureBtn", ( void* )( intptr_t )texture.GetRendererID(), ImVec2( 100, 100 ), ImVec2( 1, 1 ), ImVec2( 0, 0 ) ) )
			{
				auto entity = scene.GetEntityByID( m_EntityUUID );

				if ( !entity.has_value() )
				{
					response = WindowResponse::ERROR;
				}
				else
				{
					entity.value().GetComponent<MikuEngine::SpriteRendererComponent>().TextureIdentifier = texture.GetUUID();
					response = WindowResponse::COMPLETED;
				}
			}

			ImGui::PopID();
		}

		ImGui::EndTable();

		ImGui::End();

		if ( m_IsOpen == false ) response = WindowResponse::CLOSED;

		return response;
	}
}
