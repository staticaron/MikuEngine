#include "Windows/MaterialSelectionWindow.h"

#include "imgui.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	MaterialSelectionWindow::MaterialSelectionWindow( std::function<void( MikuEngine::UUID itemUUId )> onItemSelected ) : onItemSelected( onItemSelected ) {}

	WindowResponse MaterialSelectionWindow::RenderMaterialSelectionWindow( const std::string& identifier, const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::SetWindowSize( { 500, 500 }, ImGuiCond_FirstUseEver );

		ImGui::Begin( ( "Select Material" + identifier ).c_str(), &m_IsOpen );

		auto materials = appLevelstuff.GetAssetPoolManager().GetMaterialManager().GetAllLoadedMaterials();

		for ( const auto& [ uuid, materialContainer ] : materials )
		{
			if ( ImGui::Selectable( materialContainer.index.path.c_str() ) )
			{
				onItemSelected( uuid );
				response = WindowResponse::COMPLETED;
			}
		}

		ImGui::End();

		if ( m_IsOpen == false ) response = WindowResponse::CLOSED;

		return response;
	}
}
