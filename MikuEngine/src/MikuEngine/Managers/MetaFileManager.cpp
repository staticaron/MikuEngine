#include "Managers/MetaFileManager.h"

#include <filesystem>
#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Logger.h"

namespace MikuEngine
{
	void MetaFileManager::RefreshMetaFiles()
	{
		for ( auto file : std::filesystem::recursive_directory_iterator( PROJECT_DIR ) )
		{
			std::filesystem::file_type type;
			file.status().type( type );

			if ( file.path().extension() == ".meta" ) continue;

			if ( MetaFileExists( file.path().string() ) ) continue;

			GenerateMetaFile( file.path().string() );
		}

		MIKU_CORE_INFO( "Meta Files Refreshed!" );
	}

	void MetaFileManager::GenerateMetaFile( const std::string& filepath )
	{
		YAML::Emitter metaFileEmitter;

		metaFileEmitter << YAML::BeginMap;

		metaFileEmitter << YAML::Key << "uuid" << YAML::Value << UUID();

		metaFileEmitter << YAML::Key << "properties" << YAML::Value << YAML::BeginMap;

		metaFileEmitter << YAML::Key << "path" << YAML::Value << filepath;
		metaFileEmitter << YAML::Key << "type" << YAML::Value << "image";

		metaFileEmitter << YAML::EndMap;

		metaFileEmitter << YAML::EndMap;

		std::ofstream metaFileStream( filepath + ".meta" );
		metaFileStream << metaFileEmitter.c_str();
	}

	bool MetaFileManager::MetaFileExists( const std::string& filepath )
	{
		return std::filesystem::exists( filepath + ".meta" );
	}

	UUID MetaFileManager::GetUUIDFromMetaFile( const std::string& filepath )
	{
		auto metapath = filepath + ".meta";

		YAML::Node metaNode = YAML::LoadFile( metapath );

		return UUID( metaNode[ "uuid" ].as<std::string>() );
	}
}
