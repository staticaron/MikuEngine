#include "Managers/MetaFileManager.h"

#include <filesystem>
#include <fstream>

#include "yaml-cpp/yaml.h"

#include "UUID.h"

namespace MikuEngine
{
	void MetaFileManager::RefreshMetaFiles()
	{
		// Go through each and every directory
		for ( auto file : std::filesystem::recursive_directory_iterator( PROJECT_DIR ) )
		{
			// For each and every file check if the same directory has filename.ext.meta file
			bool metaFileExists = std::filesystem::exists( file.path().string() + ".meta" );

			// If the file exists, good
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
	}
}
