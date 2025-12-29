#pragma once

#include <string>

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API MetaFileManager
	{
	public:
		static void RefreshMetaFiles();
		static void GenerateMetaFile( const std::string& filepath );
		static bool MetaFileExists( const std::string& filepath );
		static UUID GetUUIDFromMetaFile( const std::string& filepath );

	private:
	};
}
