#include "Asset.h"

#include "Managers/ImguiManager.h"

namespace MikuEngine
{
	void Asset::RenderBaseImGui( std::string& assetName )
	{
		// Name
		ImGui::TextUnformatted( "Asset Name" );
		ImGui::SameLine();

		char assetNameContainer[ 255 ];

		std::copy( assetName.begin(), assetName.begin() + assetName.length(), assetNameContainer );
		assetNameContainer[ assetName.length() ] = '\0';

		ImGui::InputText( "##AssetBase", assetNameContainer, 255 );

		if ( ImGui::IsItemDeactivatedAfterEdit() ) assetName = std::string( assetNameContainer );

		// Delete Button

		if ( ImguiManager::FullWidthButton( "<DEL>" ) )
		{
			DeleteAsset();
		}
	}
}
