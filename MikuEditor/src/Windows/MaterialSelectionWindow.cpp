#include "Windows/MaterialSelectionWindow.h"

#include "imgui.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	MaterialSelectionWindow::MaterialSelectionWindow( MikuEngine::UUID uuid ) : m_EntityUUID( uuid ) {}

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

				entity->GetComponent<MikuEngine::SpriteRendererComponent>().MaterialUUID = uuid;
				response = WindowResponse::COMPLETED;
			}
		}

		ImGui::End();

		if ( m_IsOpen == false ) response = WindowResponse::CLOSED;

		return response;
	}
}
