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

			if ( file.path().extension() == ".meta" )
				continue;

			if ( MetaFileExists( file.path().string() ) )
				continue;

			CreateMetaFile( file.path().string(), AssetType::NONE, {} );
		}

		MIKU_CORE_INFO( "Meta Files Refreshed!" );
	}

	UUID MetaFileManager::CreateMetaFile( const std::string& filepath, AssetType assetType, const YAML::Node& properties )
	{
		YAML::Node root;

		UUID uuid;

		root[ "uuid" ] = uuid.ToString();

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

		return uuid;
	}

	/// Generate a meta file if it doesn't exists for a file
	///
	/// @param filepath path of the file to check the meta file for
	/// @param assetType type of the asset
	/// @param properties properties of the asset
	UUID MetaFileManager::GenerateMetaFileIfNotPresent( const std::string& filepath, AssetType assetType, const YAML::Node& properties )
	{
		if ( MetaFileExists( filepath ) )
			return GetUUIDFromMetaFile( filepath );
		return GenerateMetaFile( filepath, assetType, properties );
	}

	void MetaFileManager::DeleteMetaFile( const std::string& filepath )
	{
		std::filesystem::path metaFilePath = filepath + ".meta";
		if ( std::filesystem::exists( metaFilePath.c_str() ) == false )
			return;
		std::filesystem::remove( metaFilePath.c_str() );
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
