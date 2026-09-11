#pragma once

#include <optional>

#include "yaml-cpp/node/node.h"

#include "Core.h"

#include "AssetManagerBase.h"
#include "Rendering/Material.h"
#include "UUID.h"

namespace MikuEngine
{
	struct MIKU_API MaterialIndex
	{
		UUID uuid;
		std::filesystem::path path;

		std::string GetName() const { return path.stem().string(); }
	};

	struct MIKU_API MaterialContainer
	{
		MaterialIndex index;
		Material material;

		std::string GetName() const { return index.path.stem().string(); }
		void SetName( const std::string& newName )
		{
			std::filesystem::path newFilePath = index.path.parent_path() / ( newName + index.path.extension().string() );
			index.path = newFilePath;
		}
	};

	class MIKU_API MaterialManager : public AssetManagerBase
	{
	public:
		void LoadAllMaterials();

		static void CreateAssetAtPath( const std::string& name, const std::filesystem::path& path );

		void PrepareMaterialIndex();

		void Refresh();
		void RefreshMaterialIndex();
		void RefreshMaterials();

		std::optional<MaterialContainer*> GetMaterial( const UUID& uuid );
		const std::unordered_map<UUID, MaterialContainer> GetAllLoadedMaterials() const;
		std::optional<Material*> GetMaterialByFilePath( const std::string& filepath );
		const std::filesystem::path& GetFilePathFromUUID( const UUID& uuid ) override;

		bool MaterialExists( const UUID& uuid ) const;

		void RenameAssetCleanup( const UUID& uuid, const std::string& newName ) override;

		static YAML::Node GetMaterialProperties( Material* material );

	private:
		void DeleteAssetCleanup( const UUID& uuid ) override;

	private:
		std::unordered_map<UUID, MaterialIndex> m_MaterialIndex;
		std::unordered_map<UUID, MaterialContainer> m_Materials;
	};
}
