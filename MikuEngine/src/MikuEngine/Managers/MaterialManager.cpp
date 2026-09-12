#include "Managers/MaterialManager.h"

#include <filesystem>
#include <fstream>

#include "yaml-cpp/emitter.h"

#include "Application.h"
#include "Error.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void MaterialManager::Init()
	{
		LoadAllMaterials();
	}

	void MaterialManager::InitFrame()
	{
		PerformDeletions();
		PerformRenames();
	};

	void MaterialManager::LoadMaterial( const std::filesystem::path& path, UUID uuid )
	{
		m_Materials[ uuid ] = { uuid, path };
	}

	/// Load all materials from the material directory
	///
	void MaterialManager::UnloadMaterial( const std::filesystem::path& filePath )
	{
		auto material = m_Materials.begin();

		// Find the iterator with same filepath
		while ( material == m_Materials.end() )
		{
			if ( material->second.GetPath() == filePath )
				break;
			material++;
		}

		// Material not loaded! Can't unload
		if ( material == m_Materials.end() )
		{
			MIKU_CORE_DEBUG( "Tried to Remove Material named {} but no material of that name was found!", filePath.stem().c_str() );
			return;
		}

		// Delete the material
		m_Materials.erase( material );
		MetaFileManager::DeleteMetaFile( filePath );

		MIKU_CORE_DEBUG( "[UNLOADED] Material Removed named {}", filePath.stem().c_str() );
	}

	void MaterialManager::LoadAllMaterials()
	{
		const auto& data = Application::GetDataContainer();

		if ( !std::filesystem::exists( data.GetProjectAssetPath( "/materials/" ) ) )
			return;

		for ( auto& item : std::filesystem::recursive_directory_iterator( data.GetProjectAssetPath( "/materials/" ) ) )
		{
			if ( item.path().extension() == ".meta" || item.is_directory() )
				continue;

			auto uuid = MetaFileManager::GenerateMetaFileIfNotPresent( item.path().string(), AssetType::MATERIAL, GetMaterialProperties( nullptr ) );

			LoadMaterial( item.path(), uuid );
		}
	}

	/// Create an empty material asset at the provided path
	///
	/// @param name name of the material asset
	/// @param path path of the parent folder
	///
	void MaterialManager::CreateAssetAtPath( const std::string& name, const std::filesystem::path& path )
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "shader" << YAML::Value << "<NONE>";
		emitter << YAML::EndMap;

		unsigned int count = 0;
		std::filesystem::path pathToSave = path / ( name + ".mat" );

		while ( std::filesystem::exists( pathToSave ) )
		{
			count++;
			pathToSave = path / ( name + "_" + std::to_string( count ) + ".mat" );
		}

		std::ofstream fout( pathToSave );
		fout << emitter.c_str();
		fout.close();

		Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().LoadMaterial( pathToSave );

		MetaFileManager::GenerateMetaFileIfNotPresent( pathToSave, AssetType::MATERIAL, MaterialManager::GetMaterialProperties() );
	}

	/// Load the materials that are not already loaded!
	///
	void MaterialManager::RefreshMaterials( bool loadIntoExisting )
	{
		const auto& data = Application::GetDataContainer();

		if ( !std::filesystem::exists( data.GetProjectAssetPath( "/materials/" ) ) )
			return;

		unsigned int refreshCount = 0;

		for ( auto& item : std::filesystem::recursive_directory_iterator( data.GetProjectAssetPath( "/materials/" ) ) )
		{
			if ( item.path().extension() == ".meta" || item.is_directory() )
				continue;

			auto uuid = MetaFileManager::GenerateMetaFileIfNotPresent( item.path().string(), AssetType::MATERIAL, GetMaterialProperties( nullptr ) );

			if ( loadIntoExisting )
				LoadMaterial( item.path(), uuid );
			else
			{
				if ( const auto& existing = m_Materials.find( uuid ); existing != m_Materials.end() )
					continue;

				LoadMaterial( item.path(), uuid );
			}

			refreshCount++;
		}

		MIKU_CORE_INFO( "Material Index Refresh Completed with count : {}", refreshCount );
	}

	const std::unordered_map<UUID, Material> MaterialManager::GetAllMaterials() const
	{
		return m_Materials;
	}

	/// Return a material with the provided UUID
	///
	/// @param uuid uuid to the material
	///
	Material* MaterialManager::GetMaterial( const UUID& uuid )
	{
		if ( auto existingMaterial = m_Materials.find( uuid ); existingMaterial != m_Materials.end() )
			return &existingMaterial->second;

		return nullptr;
	}

	/// Return a material with the provided filepath
	///
	/// @param filepath path to the material
	///
	Material* MaterialManager::GetMaterial( const std::string& filepath )
	{
		for ( auto& [ uuid, material ] : m_Materials )
		{
			if ( material.GetPath() == filepath )
				return &material;
		}

		return nullptr;
	}

	bool MaterialManager::MaterialExists( const UUID& uuid ) const
	{
		auto exists = m_Materials.find( uuid );
		return exists != m_Materials.end();
	}

	const std::filesystem::path& MaterialManager::GetFilePathByUUID( const UUID& uuid )
	{
		if ( auto existing = m_Materials.find( uuid ); existing != m_Materials.end() )
			return existing->second.GetPath();

		MIKU_ASSERT( false, "This material is not loaded!" );
	}

	/// Get Meta File data for a Material
	///
	/// @param material a pointer to the material object! Pass nullptr to get generic data
	YAML::Node MaterialManager::GetMaterialProperties( Material* material )
	{
		return {};
	}

	void MaterialManager::AddToDeleteQueue( const UUID& uuid )
	{
		m_DeleteQueue.push_back( uuid );
	};

	void MaterialManager::AddToDeleteQueue( const std::filesystem::path& filepath )
	{
		if ( auto material = GetMaterial( filepath ); material != nullptr )
			m_DeleteQueue.push_back( material->GetUUID() );
	};

	void MaterialManager::AddToRenameQueue( const UUID& uuid, const std::string& newName )
	{
		m_RenameQueue.push_back( { uuid, newName } );
	};

	void MaterialManager::AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName )
	{
		if ( auto material = GetMaterial( filepath ); material != nullptr )
			m_RenameQueue.push_back( { material->GetUUID(), newName } );
	};

	void MaterialManager::PerformDeletions()
	{
		if ( m_DeleteQueue.size() <= 0 )
			return;

		for ( auto uuid : m_DeleteQueue )
			DeleteAsset( uuid );

		MIKU_CORE_INFO( "{} Materils Deleted", m_DeleteQueue.size() );

		m_DeleteQueue.clear();
	};

	void MaterialManager::PerformRenames()
	{
		if ( m_RenameQueue.size() <= 0 )
			return;

		for ( auto [ uuid, newName ] : m_RenameQueue )
			RenameAsset( uuid, newName );

		MIKU_CORE_INFO( "{} Materils Renamed", m_RenameQueue.size() );

		m_RenameQueue.clear();
	};

	void MaterialManager::DeleteAsset( const UUID& uuid )
	{
		auto material = m_Materials.find( uuid );

		if ( material == m_Materials.end() )
			return;

		auto filepath = material->second.GetPath();

		// Remove the material from DB
		m_Materials.erase( material );

		// Delete the physical files
		if ( std::filesystem::exists( filepath ) )
			std::filesystem::remove( filepath );
		if ( std::filesystem::exists( filepath.string() + ".meta" ) )
			std::filesystem::remove( filepath.string() + ".meta" );
	};

	void MaterialManager::RenameAsset( const UUID& uuid, const std::string& newName )
	{
		auto materialToRename = m_Materials.find( uuid );

		if ( materialToRename == m_Materials.end() )
			return;

		const std::filesystem::path& filePath = GetFilePathByUUID( uuid );
		const std::string fileExtension = filePath.extension();

		std::filesystem::path newFilePath = filePath.parent_path() / ( newName + fileExtension );
		std::filesystem::path newMetaFilePath = filePath.parent_path() / ( newName + fileExtension + ".meta" );

		std::filesystem::rename( filePath, newFilePath );
		std::filesystem::rename( filePath.string() + ".meta", newMetaFilePath );

		materialToRename->second.SetPath( newFilePath );
	};

}
