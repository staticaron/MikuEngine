#include "Windows/ModelSelectionWindow.h"

#include "imgui.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	ModelSelectionWindow::ModelSelectionWindow( std::function<void( MikuEngine::UUID )> onModelSelection ) : m_OnModelSelection( onModelSelection ) {}

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
						m_OnModelSelection( uuid );
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
						m_OnModelSelection( uuid );
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
