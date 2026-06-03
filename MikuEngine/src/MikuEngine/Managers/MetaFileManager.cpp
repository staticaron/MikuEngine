#include "Managers/MetaFileManager.h"

#include <filesystem>
#include <fstream>

#include "Application.h"
#include "Data/AssetType.h"
#include "Logger.h"
#include "UUID.h"

namespace MikuEngine
{
	void MetaFileManager::RefreshMetaFiles()
	{
		const auto& dataContainer = Application::GetDataContainer();

		for ( auto file : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectDir() ) )
		{
			std::filesystem::file_type type;
			file.status().type( type );

			if ( file.path().extension() == ".meta" ) continue;

			if ( MetaFileExists( file.path().string() ) ) continue;

			GenerateMetaFile( file.path().string(), AssetType::NONE, {} );
		}

		MIKU_CORE_INFO( "Meta Files Refreshed!" );
	}

	void MetaFileManager::GenerateMetaFile( const std::string& filepath, AssetType assetType, const YAML::Node& properties )
	{
		YAML::Node root;

		root[ "uuid" ] = UUID().ToString();

		root[ "properties" ][ "path" ] = filepath;

		switch ( assetType )
		{
		case AssetType::TEXTURE:
			root[ "properties" ][ "type" ] = "texture";
			break;
		case AssetType::MATERIAL:
			root[ "properties" ][ "type" ] = "material";
			break;
		case AssetType::SHADER:
			root[ "properties" ][ "type" ] = "shader";
			break;
		case AssetType::MODEL:
			root[ "properties" ][ "type" ] = "model";
			break;
		default:
			root[ "properties" ][ "type" ] = "none";
			break;
		}

		for ( auto it = properties.begin(); it != properties.end(); it++ )
		{
			root[ "properties" ][ it->first ] = it->second;
		}

		std::ofstream metaFileStream( filepath + ".meta" );
		metaFileStream << root;
		metaFileStream.close();
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

	std::optional<YAML::Node> MetaFileManager::GetMetaFileNode( const std::filesystem::path& filepath )
	{
		if ( MetaFileManager::MetaFileExists( filepath ) )
		{
			auto metapath = filepath.string() + ".meta";
			return YAML::LoadFile( metapath );
		}

		return std::nullopt;
	}
}
