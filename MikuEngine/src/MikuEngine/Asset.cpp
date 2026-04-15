#include "Asset.h"

#include "Managers/ImguiManager.h"

namespace MikuEngine
{
	void Asset::RenderInspectorImGui()
	{
		auto oldName = GetName();
		std::string newName = oldName;

		// RENDER ASSET NAME FIELD
		//
		//
		ImGui::TextUnformatted( "Asset Name" );
		ImGui::SameLine();

		char assetNameContainer[ 255 ];

		std::copy( newName.begin(), newName.begin() + newName.length(), assetNameContainer );
		assetNameContainer[ newName.length() ] = '\0';

		ImGui::InputText( "##AssetBase", assetNameContainer, 255 );

		if ( ImGui::IsItemDeactivatedAfterEdit() ) newName = std::string( assetNameContainer );

		if ( oldName != newName )
		{
			SetName( newName );
		}

		// RENDER THE ASSET IMGUI
		//
		//
		AssetImGui();

		// RENDER DELETE ASSET BUTTON
		//
		//
		if ( ImguiManager::FullWidthButton( "<DEL>" ) )
		{
			DeleteAsset();
		}
	}
}
