#pragma once

#include <filesystem>

#include "Core.h"
#include "Data/EngineConfig.h"

namespace MikuEngine
{
	const std::string DEFAULT_ENGINE_CONFIG_LOCATION = "EngineConfig.yaml";

	class MIKU_API EngineConfigLoader
	{
	public:
		static EngineConfig LoadConfig( const std::filesystem::path& configFilePath = DEFAULT_ENGINE_CONFIG_LOCATION );
		static void SaveConfig( const EngineConfig& engineConfig, const std::filesystem::path& configFilePath = DEFAULT_ENGINE_CONFIG_LOCATION );

	private:
	};
}
