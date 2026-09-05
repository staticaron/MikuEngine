#pragma once

#include <filesystem>

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API IAssetManagerBase
	{
	public:
		virtual void InitFrame() = 0;

		virtual void AddToDeleteQueue( const UUID& uuid ) = 0;
		virtual void AddToDeleteQueue( const std::filesystem::path& filepath ) = 0;

		virtual void AddToRenameQueue( const UUID& uuid, const std::string& newName ) = 0;
		virtual void AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName ) = 0;

	protected:
		virtual void PerformDeletions() = 0;

		virtual void DeleteAsset( const UUID& uuid ) = 0;
		virtual void RenameAsset( const UUID& uuid, const std::string& newName ) = 0;

		virtual const std::filesystem::path& GetFilePathByUUID( const UUID& uuid ) = 0;
	};
}
