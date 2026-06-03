#include "EngineConfigLoader.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Logger.h"

namespace MikuEngine
{
	EngineConfig EngineConfigLoader::LoadConfig( const std::filesystem::path& configLocation )
	{
		EngineConfig engineConfig;

		if ( std::filesystem::exists( configLocation ) == false )
		{
			MIKU_CORE_WARN( "Config File Not Found! Loading Defaults" );
			SaveConfig( {}, configLocation );
			return {};
		}

		auto rootNode = YAML::LoadFile( configLocation );
		if ( rootNode[ "project-folder-location" ] ) engineConfig.ProjectFolderLocation = rootNode[ "project-folder-location" ].as<std::string>();

		MIKU_CORE_DEBUG( "Config File Loaded!" );

		return engineConfig;
	}

	void EngineConfigLoader::SaveConfig( const EngineConfig& engineConfig, const std::filesystem::path& savePath )
	{
		YAML::Node rootNode;
		rootNode[ "project-folder-location" ] = engineConfig.ProjectFolderLocation;

		YAML::Emitter emitter;
		emitter << rootNode;

		std::ofstream engineConfigFile( savePath );
		engineConfigFile << emitter.c_str();
	}
}
