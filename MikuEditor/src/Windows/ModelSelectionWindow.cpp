#include "Windows/ModelSelectionWindow.h"

#include "imgui.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	ModelSelectionWindow::ModelSelectionWindow( MikuEngine::UUID uuid ) : m_EntityUUID( uuid ) {}

	WindowResponse ModelSelectionWindow::RenderModelSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::Begin( "Select Model", &m_IsOpen );

		if ( ImGui::BeginTabBar( "Select Model" ) )
		{
			if ( ImGui::BeginTabItem( "Project" ) )
			{
				auto models = appLevelstuff.GetAssetPoolManager().GetModelManager().GetAllLoadedModels();

				for ( const auto& [ uuid, modelContainer ] : models )
				{
					if ( ImGui::Selectable( modelContainer.index.path.c_str() ) )
					{
						auto entity = scene.GetEntityByID( m_EntityUUID );

						if ( entity.has_value() == false )
						{
							response = WindowResponse::ERROR;
							break;
						}

						entity->GetComponent<MikuEngine::MeshRendererComponent>().ModelIdentifier = uuid;
						response = WindowResponse::COMPLETED;
					}
				}

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem( "Default" ) )
			{
				auto defaultModels = appLevelstuff.GetAssetPoolManager().GetModelManager().GetAllDefaultModels();

				for ( const auto& [ uuid, modelContainer ] : defaultModels )
				{
					if ( ImGui::Selectable( modelContainer.index.path.c_str() ) )
					{
						auto entity = scene.GetEntityByID( m_EntityUUID );

						if ( entity.has_value() == false )
						{
							response = WindowResponse::ERROR;
							break;
						}

						entity->GetComponent<MikuEngine::MeshRendererComponent>().ModelIdentifier = uuid;
						response = WindowResponse::COMPLETED;
					}
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();

		if ( m_IsOpen == false ) response = WindowResponse::CLOSED;

		return response;
	}
}
