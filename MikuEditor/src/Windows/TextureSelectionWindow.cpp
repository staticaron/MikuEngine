#include "Windows/TextureSelectionWindow.h"

#include "imgui.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Logger.h"

namespace MikuEditor
{
	TextureSelectionWindow::TextureSelectionWindow( MikuEngine::UUID entityUUID, std::function<void( MikuEngine::UUID )> selectionCallback ) : m_EntityUUID( entityUUID ), m_TextureSetCallback( selectionCallback ) {}

	TextureSelectionWindowResponse TextureSelectionWindow::RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		TextureSelectionWindowResponse response;

		ImGui::Begin( "Select Texture", &m_IsOpen );

		auto canvasSize = ImGui::GetContentRegionAvail();
		auto columns = static_cast<unsigned int>( canvasSize.x / m_ImageSize );

		// Render each texture as image button in table
		ImGui::BeginTable( "Texture Button Grid", columns );

		auto allTextures = appLevelStuff.GetTextureManager().GetAllLoadedTextures();

		for ( auto [ uuid, texture ] : allTextures )
		{
			ImGui::TableNextColumn();
			ImGui::PushID( texture.GetUUID() );

			if ( ImGui::ImageButton( "##TextureBtn", ( void* )( intptr_t )texture.GetRendererID(), ImVec2( 100, 100 ), ImVec2( 1, 1 ), ImVec2( 0, 0 ) ) )
			{
				auto entity = scene.GetEntityByID( m_EntityUUID );

				if ( !entity.has_value() )
				{
					MIKU_CLIENT_ERROR( "Entity for which this texture selection window was opened doesn't exists anymore!" );
					response = TextureSelectionWindowResponse::ERROR;
				}
				else
				{
					entity.value().GetComponent<MikuEngine::SpriteRendererComponent>().TextureIdentifier = texture.GetUUID();
					response = TextureSelectionWindowResponse::COMPLETED;
				}
			}

			ImGui::PopID();
		}

		ImGui::EndTable();

		ImGui::End();

		if ( m_IsOpen == false ) response = TextureSelectionWindowResponse::CLOSED;

		return response;
	}
}
