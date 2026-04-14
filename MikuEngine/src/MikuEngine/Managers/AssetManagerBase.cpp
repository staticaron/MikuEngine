#include "Managers/AssetManagerBase.h"

#include "UUID.h"

namespace MikuEngine
{
	void AssetManagerBase::DeleteAsset( const UUID& uuid )
	{
		const std::filesystem::path& filePath = GetFilePathFromUUID( uuid );

		if ( std::filesystem::exists( filePath ) ) std::filesystem::remove( filePath );
		if ( std::filesystem::exists( filePath.string() + ".meta" ) ) std::filesystem::remove( filePath.string() + ".meta" );

		DeleteAssetCleanup( uuid );
	}

	void AssetManagerBase::RenameAsset( const UUID& uuid, const std::string& newName )
	{
		const std::filesystem::path& filePath = GetFilePathFromUUID( uuid );
		const std::string fileExtension = filePath.extension();

		std::filesystem::path newFilePath = filePath.parent_path() / ( newName + fileExtension );
		std::filesystem::path newMetaFilePath = filePath.parent_path() / ( newName + fileExtension + ".meta" );

		std::filesystem::rename( filePath, newFilePath );
		std::filesystem::rename( filePath.string() + ".meta", newMetaFilePath );

		RenameAssetCleanup( uuid, newName );
	}
}
