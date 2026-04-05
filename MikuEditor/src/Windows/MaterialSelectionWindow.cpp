#include "Windows/MaterialSelectionWindow.h"

#include "imgui.h"

#include "Entity.h"
#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	MaterialSelectionWindow::MaterialSelectionWindow( MikuEngine::UUID uuid, std::function<void( MikuEngine::Scene& scene, MikuEngine::UUID itemUUId )> onItemSelected ) : m_EntityUUID( uuid ), onItemSelected( onItemSelected ) {}

	WindowResponse MaterialSelectionWindow::RenderMaterialSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::Begin( "Select Material", &m_IsOpen );

		auto materials = appLevelstuff.GetAssetPoolManager().GetMaterialManager().GetAllLoadedMaterials();

		for ( const auto& [ uuid, materialContainer ] : materials )
		{
			if ( ImGui::Selectable( materialContainer.index.path.c_str() ) )
			{
				auto entity = scene.GetEntityByID( m_EntityUUID );

				if ( entity.has_value() == false )
				{
					response = WindowResponse::ERROR;
					break;
				}

				onItemSelected( scene, uuid );
				response = WindowResponse::COMPLETED;
			}
		}

		ImGui::End();

		if ( m_IsOpen == false ) response = WindowResponse::CLOSED;

		return response;
	}
}
