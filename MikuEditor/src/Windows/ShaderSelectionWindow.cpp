#include "Windows/ShaderSelectionWindow.h"

#include "imgui.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	ShaderSelectionWindow::ShaderSelectionWindow( std::function<void( MikuEngine::UUID itemUUID )> onShaderSelection ) : onShaderSelection( onShaderSelection ) {}

	WindowResponse ShaderSelectionWindow::RenderShaderSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::Begin( "Select Shader", &m_IsOpen );

		if ( ImGui::BeginTabBar( "Shader Selection Tabs" ) )
		{
			if ( ImGui::BeginTabItem( "Project" ) )
			{
				auto projectShaders = appLevelstuff.GetAssetPoolManager().GetShaderManager().GetAllLoadedShaders();

				for ( const auto& [ uuid, shaderContainer ] : projectShaders )
				{
					if ( ImGui::Selectable( shaderContainer.index.path.c_str() ) )
					{
						onShaderSelection( uuid );
						response = WindowResponse::COMPLETED;
					}
				}

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem( "Default" ) )
			{
				auto defaultShaders = appLevelstuff.GetAssetPoolManager().GetShaderManager().GetAllDefaultShaders();

				for ( const auto& [ uuid, shaderContainer ] : defaultShaders )
				{
					if ( ImGui::Selectable( shaderContainer.index.path.c_str() ) )
					{
						onShaderSelection( uuid );
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
