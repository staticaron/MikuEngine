#pragma once

#include <filesystem>
#include <optional>

#include "yaml-cpp/yaml.h"

#include "Core.h"
#include "UUID.h"

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

		template <typename T>
		static T GetValueFromMetaFile( const std::filesystem::path& filePath, const std::vector<std::string>& identifier )
		{
			auto metapath = filePath.string() + ".meta";
			YAML::Node metaNode = YAML::LoadFile( metapath );

			YAML::Node currentNode = metaNode;

			for ( const auto& node : identifier )
			{
				currentNode = currentNode[ node ];
			}

			return currentNode.as<T>();
		}

		static std::optional<YAML::Node> GetMetaFileNode( const std::filesystem::path& filepath );

	private:
	};
}
