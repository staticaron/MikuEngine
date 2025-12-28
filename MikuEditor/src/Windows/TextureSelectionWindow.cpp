#include "Windows/TextureSelectionWindow.h"

#include "MikuEngine/Entity.h"

namespace MikuEditor
{
	TextureSelectionWindowResponse TextureSelectionWindow::RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelStuff, MikuEngine::Scene& scene )
	{
		ImGui::Begin( "Select Texture", &m_IsOpen );

		if ( ImGui::Button( "MIKU" ) )
		{
			auto entity = scene.GetEntityByID( m_EntityUUID );

			if ( !entity.has_value() ) return TextureSelectionWindowResponse::ERROR;

			auto texture = appLevelStuff.GetTextureManager().GetTexture( "miku" );

			m_TextureSetCallback( texture.GetUUID() );

			return TextureSelectionWindowResponse::COMPLETED;
		}

		ImGui::End();

		if ( m_IsOpen == false ) return TextureSelectionWindowResponse::CLOSED;

		return TextureSelectionWindowResponse::INPROGRESS;
	}
}
