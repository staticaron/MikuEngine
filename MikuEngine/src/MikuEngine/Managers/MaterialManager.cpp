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

	void MaterialManager::LoadAllMaterials()
	{
		PrepareMaterialIndex();

		for ( const auto& [ uuid, index ] : m_MaterialIndex )
		{
			Material material( uuid, index.path );
			m_Materials[ uuid ] = { index, material };
		}
	}

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

		Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().Refresh();
	}

	void MaterialManager::PrepareMaterialIndex()
	{
		const auto& dataContainer = Application::GetDataContainer();

		if ( !std::filesystem::exists( dataContainer.GetProjectAssetPath( "/materials/" ) ) )
			return;

		m_MaterialIndex.clear();

		for ( auto& file : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectAssetPath( "/materials/" ) ) )
		{
			if ( file.path().extension() == ".meta" )
				continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) )
				MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::MATERIAL, GetMaterialProperties( nullptr ) );

			if ( file.is_directory() )
				continue;

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_MaterialIndex[ uuid ] = { uuid, file.path().string() };
		}
	}

	void MaterialManager::Refresh()
	{
		RefreshMaterialIndex();
		RefreshMaterials();
	}

	void MaterialManager::RefreshMaterialIndex()
	{
		const auto& dataContainer = Application::GetDataContainer();

		if ( !std::filesystem::exists( dataContainer.GetProjectAssetPath( "/materials/" ) ) )
			return;

		unsigned int refreshCount = 0;

		for ( auto& file : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectAssetPath( "/materials/" ) ) )
		{
			if ( file.is_directory() )
				continue;
			if ( file.path().extension() == ".meta" )
				continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) )
				MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::MATERIAL, GetMaterialProperties( nullptr ) );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );

			const auto& existingIndex = m_MaterialIndex.find( uuid );

			if ( existingIndex != m_MaterialIndex.end() )
				continue;

			m_MaterialIndex[ uuid ] = { uuid, file.path().string() };

			refreshCount++;
		}

		MIKU_CORE_INFO( "Material Index Refresh Completed with count : {}", refreshCount );
	}

	void MaterialManager::RefreshMaterials()
	{
		unsigned int refreshCount = 0;

		for ( const auto& [ uuid, index ] : m_MaterialIndex )
		{
			const auto& existing = m_Materials.find( uuid );

			if ( existing != m_Materials.end() )
				continue;

			Material material( uuid, index.path );
			m_Materials[ uuid ] = { index, material };

			refreshCount++;
		}
	}

	const std::unordered_map<UUID, MaterialContainer> MaterialManager::GetAllLoadedMaterials() const
	{
		return m_Materials;
	}

	std::optional<MaterialContainer*> MaterialManager::GetMaterial( const UUID& uuid )
	{
		if ( auto existingMaterial = m_Materials.find( uuid ); existingMaterial != m_Materials.end() )
			return &existingMaterial->second;
		else
			return std::nullopt;
	}

	std::optional<Material*> MaterialManager::GetMaterialByFilePath( const std::string& filepath )
	{
		for ( auto [ uuid, materialContainer ] : m_Materials )
		{
			if ( materialContainer.index.path == filepath )
			{
				return &materialContainer.material;
			}
		}

		return {};
	}

	bool MaterialManager::MaterialExists( const UUID& uuid ) const
	{
		auto exists = m_Materials.find( uuid );
		return exists != m_Materials.end();
	}

	const std::filesystem::path& MaterialManager::GetFilePathByUUID( const UUID& uuid )
	{
		if ( auto existing = m_Materials.find( uuid ); existing != m_Materials.end() )
		{
			return existing->second.index.path;
		}

		MIKU_ASSERT( false, "This material is not loaded!" );
	}

	YAML::Node MaterialManager::GetMaterialProperties( Material* material )
	{
		return {};
	}

	void MaterialManager::InitFrame()
	{
		PerformDeletions();
		PerformRenames();
	};

	void MaterialManager::AddToDeleteQueue( const UUID& uuid )
	{
		m_DeleteQueue.push_back( uuid );
	};

	void MaterialManager::AddToDeleteQueue( const std::filesystem::path& filepath )
	{
		if ( auto material = GetMaterialByFilePath( filepath ); material.has_value() )
			m_DeleteQueue.push_back( material.value()->GetUUID() );
	};

	void MaterialManager::AddToRenameQueue( const UUID& uuid, const std::string& newName )
	{
		m_RenameQueue.push_back( { uuid, newName } );
	};

	void MaterialManager::AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName )
	{
		if ( auto material = GetMaterialByFilePath( filepath ); material.has_value() )
			m_RenameQueue.push_back( { material.value()->GetUUID(), newName } );
	};

	void MaterialManager::PerformDeletions()
	{
		for ( auto uuid : m_DeleteQueue )
			DeleteAsset( uuid );

		m_DeleteQueue.clear();
	};

	void MaterialManager::PerformRenames()
	{
		for ( auto [ uuid, newName ] : m_RenameQueue )
			RenameAsset( uuid, newName );

		m_RenameQueue.clear();
	};

	void MaterialManager::DeleteAsset( const UUID& uuid )
	{
		auto material = m_Materials.find( uuid );

		if ( material == m_Materials.end() )
			return;

		auto filepath = material->second.index.path;

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
		const std::filesystem::path& filePath = GetFilePathByUUID( uuid );
		const std::string fileExtension = filePath.extension();

		std::filesystem::path newFilePath = filePath.parent_path() / ( newName + fileExtension );
		std::filesystem::path newMetaFilePath = filePath.parent_path() / ( newName + fileExtension + ".meta" );

		std::filesystem::rename( filePath, newFilePath );
		std::filesystem::rename( filePath.string() + ".meta", newMetaFilePath );

		if ( auto existing = m_Materials.find( uuid ); existing != m_Materials.end() )
			existing->second.SetName( newName );
	};

}
