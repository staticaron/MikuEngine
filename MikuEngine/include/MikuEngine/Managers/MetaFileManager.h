#pragma once

#include <string>

#include "Core.h"

namespace MikuEngine
{
	class UUID;
}

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
