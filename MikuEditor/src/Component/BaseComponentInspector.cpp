#include "Component/BaseComponentInspector.h"

#include "Logger.h"
#include "Managers/ImguiManager.h"

namespace MikuEditor
{
	void BaseComponentInspector::ComponentHeader( std::function<std::string()> getNameCallback, std::function<void( std::string )> setNameCallback )
	{
		std::string oldName = getNameCallback();
		std::string newName = oldName;

		ImGui::TextUnformatted( "Asset Name" );
		ImGui::SameLine();

		char assetNameContainer[ 255 ];

		std::copy( newName.begin(), newName.begin() + newName.length(), assetNameContainer );
		assetNameContainer[ newName.length() ] = '\0';

		ImGui::InputText( "##AssetBase", assetNameContainer, 255 );

		if ( ImGui::IsItemDeactivatedAfterEdit() && assetNameContainer[ 0 ] != '\0' )
			newName = std::string( assetNameContainer );

		if ( ImGui::IsItemDeactivatedAfterEdit() && assetNameContainer[ 0 ] == '\0' )
			MIKU_CORE_WARN( "Failed to rename the asset! AssetName is empty!" );

		if ( oldName != newName )
			setNameCallback( newName );

		ImGui::Separator();
	}

	void BaseComponentInspector::ComponentFooter( std::function<void()> deleteAssetCallback )
	{
		ImGui::Separator();

		if ( MikuEngine::ImguiManager::FullWidthButton( "<DEL>" ) )
			deleteAssetCallback();
	}
}
