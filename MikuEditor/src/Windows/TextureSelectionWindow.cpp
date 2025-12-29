#include "Windows/TextureSelectionWindow.h"

#include "MikuEngine/Entity.h"
#include "MikuEngine/Logger.h"

namespace MikuEditor
{
	TextureSelectionWindow::TextureSelectionWindow( MikuEngine::UUID entityUUID, std::function<void( MikuEngine::UUID )> selectionCallback ) : m_EntityUUID( entityUUID ), m_TextureSetCallback( selectionCallback ) {}

	TextureSelectionWindowResponse TextureSelectionWindow::RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		TextureSelectionWindowResponse response;

		ImGui::Begin( "Select Texture", &m_IsOpen );

		if ( ImGui::Button( "MIKU" ) )
		{
			auto entity = scene.GetEntityByID( m_EntityUUID );

			if ( !entity.has_value() )
			{
				MIKU_CLIENT_ERROR( "Entity for which this texture selection window was opened doesn't exists anymore!" );
				response = TextureSelectionWindowResponse::ERROR;
			}
			else
			{
				auto texture = appLevelStuff.GetTextureManager().GetTexture( "miku" );
				entity.value().GetComponent<MikuEngine::SpriteRendererComponent>().TextureIdentifier = texture.GetUUID();
				response = TextureSelectionWindowResponse::COMPLETED;
			}
		}

		ImGui::End();

		if ( m_IsOpen == false ) response = TextureSelectionWindowResponse::CLOSED;

		return response;
	}
}
