#pragma once

#include <filesystem>
#include <optional>

#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class UUID;
	enum class AssetType;
}

namespace MikuEngine
{
	class MIKU_API MetaFileManager
	{
	public:
		static void RefreshMetaFiles();
		static UUID GenerateMetaFile( const std::string& filepath, AssetType assetType, const YAML::Node& content );
		static UUID GenerateMetaFileIfNotPresent( const std::string& filepath, AssetType assetType, const YAML::Node& properties );

		static UUID CreateMetaFile( const std::string& filepath, AssetType assetType, const YAML::Node& content );
		static void DeleteMetaFile( const std::string& filepath );

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
