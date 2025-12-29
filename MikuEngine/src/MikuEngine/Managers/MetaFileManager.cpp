#include "Managers/MetaFileManager.h"

#include <filesystem>
#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Logger.h"
#include "UUID.h"

namespace MikuEngine
{
	void MetaFileManager::RefreshMetaFiles()
	{
		for ( auto file : std::filesystem::recursive_directory_iterator( PROJECT_DIR ) )
		{
			std::filesystem::file_type type;
			file.status().type( type );

			// Skip meta files
			if ( file.path().extension() == ".meta" ) continue;

			// If meta file already exists, Skip
			bool metaFileExists = std::filesystem::exists( file.path().string() + ".meta" );
			if ( metaFileExists ) continue;

			// If it doesn't, generate the file with the required meta data and random UUID
			YAML::Emitter metaFileEmitter;

			metaFileEmitter << YAML::BeginMap;

			metaFileEmitter << YAML::Key << "uuid" << YAML::Value << UUID();

			metaFileEmitter << YAML::Key << "properties" << YAML::Value << YAML::BeginMap;

			metaFileEmitter << YAML::Key << "path" << YAML::Value << file.path().string();
			metaFileEmitter << YAML::Key << "type" << YAML::Value << "image";

			metaFileEmitter << YAML::EndMap;

			metaFileEmitter << YAML::EndMap;

			std::ofstream metaFileStream( file.path().string() + ".meta" );
			metaFileStream << metaFileEmitter.c_str();
		}

		MIKU_INFO( "Meta Files Refreshed!" );
	}
}
