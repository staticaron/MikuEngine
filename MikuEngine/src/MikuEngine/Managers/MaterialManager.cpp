#include "Managers/MaterialManager.h"

#include <filesystem>

#include "Error.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void MaterialManager::LoadAllMaterials()
	{
		PrepareMaterialIndex();

		for ( const auto& [ uuid, index ] : m_MaterialIndex )
		{
			Material material( uuid, index.path );
			m_Materials[ uuid ] = { index, material };
		}
	}

	void MaterialManager::PrepareMaterialIndex()
	{
		if ( !std::filesystem::exists( PROJECT_DIR "/materials/" ) ) return;

		m_MaterialIndex.clear();

		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/materials/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

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
		if ( !std::filesystem::exists( PROJECT_DIR "/materials/" ) ) return;

		unsigned int refreshCount = 0;

		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/materials/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );

			const auto& existingIndex = m_MaterialIndex.find( uuid );

			if ( existingIndex != m_MaterialIndex.end() ) continue;

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

			if ( existing != m_Materials.end() ) continue;

			Material material( uuid, index.path );
			m_Materials[ uuid ] = { index, material };

			refreshCount++;
		}

		MIKU_CORE_INFO( "Material Refresh Completed with count : {}", refreshCount );
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

	const std::filesystem::path& MaterialManager::GetFilePathFromUUID( const UUID& uuid )
	{
		if ( auto existing = m_Materials.find( uuid ); existing != m_Materials.end() )
		{
			return existing->second.index.path;
		}

		MIKU_ASSERT( false, "This material is not loaded!" );
	}

	void MaterialManager::RenameAssetCleanup( const UUID& uuid, const std::string& newName )
	{
		if ( auto existing = m_Materials.find( uuid ); existing != m_Materials.end() )
		{
			existing->second.SetName( newName );
		}
	}

	void MaterialManager::DeleteAssetCleanup( const UUID& uuid )
	{
		if ( auto existing = m_Materials.find( uuid ); existing != m_Materials.end() )
		{
			m_Materials.erase( existing );
		}
	}
}
