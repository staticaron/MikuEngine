#pragma once

#include <filesystem>

#include "Core.h"

namespace MikuEngine
{
	class UUID;
}

namespace MikuEngine
{
	class MIKU_API AssetManagerBase
	{
	public:
		void DeleteAsset( const UUID& uuid );
		void RenameAsset( const UUID& uuid, const std::string& newName );

		virtual const std::filesystem::path& GetFilePathFromUUID( const UUID& uuid ) = 0;

		virtual void RenameAssetCleanup( const UUID& uuid, const std::string& newName ) = 0;
		virtual void DeleteAssetCleanup( const UUID& uuid ) = 0;

		virtual void AddToDeleteQueue( const UUID& uuid ) = 0;
		virtual void PerformDeletions() = 0;

	private:
	};
}
