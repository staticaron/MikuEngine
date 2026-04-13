#pragma once

#include <optional>

#include "Core.h"

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
	};

	class MIKU_API MaterialManager
	{
	public:
		void LoadAllMaterials();
		void LoadMaterial( Material mat );

		void PrepareMaterialIndex();

		void Refresh();
		void RefreshMaterialIndex();
		void RefreshMaterials();

		const std::unordered_map<UUID, MaterialContainer> GetAllLoadedMaterials() const;

		std::optional<MaterialContainer*> GetMaterial( const UUID& uuid );
		bool MaterialExists( const UUID& uuid ) const;

		std::optional<Material*> GetMaterialByFilePath( const std::string& filepath );

	private:
		std::unordered_map<UUID, MaterialIndex> m_MaterialIndex;
		std::unordered_map<UUID, MaterialContainer> m_Materials;
	};
}
