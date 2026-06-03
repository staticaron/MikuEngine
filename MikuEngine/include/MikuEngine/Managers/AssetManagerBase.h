#pragma once

#include <filesystem>

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API AssetManagerBase
	{
	public:
		virtual ~AssetManagerBase() = default;

		virtual void InitFrame();

		void DeleteAsset( const UUID& uuid );
		void RenameAsset( const UUID& uuid, const std::string& newName );

		virtual const std::filesystem::path& GetFilePathFromUUID( const UUID& uuid ) = 0;

		virtual void RenameAssetCleanup( const UUID& uuid, const std::string& newName ) = 0;
		virtual void DeleteAssetCleanup( const UUID& uuid ) = 0;

		virtual void AddToDeleteQueue( const UUID& uuid ) { m_DeleteQueue.push_back( uuid ); }
		void PerformDeletions();

	protected:
		std::vector<UUID> m_DeleteQueue;
	};
}
