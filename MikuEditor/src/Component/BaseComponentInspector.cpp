#include "Component/BaseComponentInspector.h"

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

		if ( ImGui::IsItemDeactivatedAfterEdit() ) newName = std::string( assetNameContainer );

		if ( oldName != newName ) setNameCallback( newName );

		ImGui::Separator();
	}

	void BaseComponentInspector::ComponentFooter( std::function<void()> deleteAssetCallback )
	{
		ImGui::Separator();

		if ( MikuEngine::ImguiManager::FullWidthButton( "<DEL>" ) ) deleteAssetCallback();
	}
}
