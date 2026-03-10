#include "Managers/MaterialManager.h"

#include <filesystem>

#include "Error.h"
#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void MaterialManager::LoadAllMaterials()
	{
		PrepareMaterialIndex();

		for ( const auto& [ uuid, index ] : m_MaterialIndex )
		{
			Material material( uuid );
			material.LoadFromFile( index.path );
			m_Materials[ uuid ] = material;

			MIKU_CORE_DEBUG( "Material Loaded into memory : {}", m_Materials.at( uuid ).GetUUID().ToString() );
		}
	}

	void MaterialManager::PrepareMaterialIndex()
	{
		if ( !std::filesystem::exists( PROJECT_DIR "/materials/" ) ) return;

		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/materials/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_MaterialIndex[ uuid ] = { uuid, file.path().stem().string(), file.path().string() };
		}
	}

	std::optional<Material*> MaterialManager::GetMaterial( UUID uuid )
	{
		auto exists = m_Materials.find( uuid );
		if ( exists == m_Materials.end() ) return {};

		return &exists->second;
	}

	std::optional<Material*> MaterialManager::GetMaterialByFilePath( const std::string& filepath )
	{
		for ( auto [ uuid, materialIndex ] : m_MaterialIndex )
			if ( materialIndex.path == filepath ) return { GetMaterial( uuid ) };

		return {};
	}

	bool MaterialManager::MaterialExists( const UUID& uuid ) const
	{
		auto exists = m_Materials.find( uuid );
		return exists != m_Materials.end();
	}
}
