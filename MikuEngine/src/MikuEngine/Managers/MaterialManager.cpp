#include "Managers/MaterialManager.h"

#include <filesystem>

#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void MaterialManager::LoadAllMaterials()
	{
		for ( const auto& [ uuid, index ] : m_MaterialIndex )
		{
			Material material;
			material.LoadFromFile( index.path );
			m_Materials[ uuid ] = material;
		}
	}

	void MaterialManager::PrepareMaterialIndex()
	{
		if ( !std::filesystem::exists( PROJECT_DIR "/materials/" ) ) return;

		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/materials/" ) )
		{
			if ( file.path().extension() != ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_MaterialIndex[ uuid ] = { uuid, file.path().stem().string(), file.path().string() };
		}
	}

	std::optional<Material> MaterialManager::GetMaterial( UUID uuid ) const
	{
		auto exists = m_Materials.find( uuid );
		if ( exists == m_Materials.end() ) return {};

		return exists->second;
	}

	bool MaterialManager::MaterialExists( const UUID& uuid ) const
	{
		auto exists = m_Materials.find( uuid );
		return exists != m_Materials.end();
	}
}
